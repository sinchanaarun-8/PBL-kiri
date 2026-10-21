from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import ctypes
import json

BASE = Path(__file__).resolve().parent
LIB_PATH = BASE / "build" / "libsimulator.so"
lib = ctypes.CDLL(str(LIB_PATH))

INT_P = ctypes.POINTER(ctypes.c_int)

lib.bridge_init.argtypes = [
    ctypes.c_int, ctypes.c_int, INT_P, INT_P, INT_P
]
lib.bridge_init.restype = ctypes.c_int

lib.bridge_check_safety.argtypes = [INT_P, INT_P, INT_P]
lib.bridge_check_safety.restype = ctypes.c_int

lib.bridge_request.argtypes = [
    ctypes.c_int, INT_P, INT_P, INT_P, INT_P
]
lib.bridge_request.restype = ctypes.c_int

lib.bridge_set_waiting_request.argtypes = [ctypes.c_int, INT_P]
lib.bridge_set_waiting_request.restype = ctypes.c_int

lib.bridge_detect_deadlock.argtypes = [INT_P]
lib.bridge_detect_deadlock.restype = ctypes.c_int

lib.bridge_recover.argtypes = [ctypes.c_int]
lib.bridge_recover.restype = ctypes.c_int

lib.bridge_get_state.argtypes = [INT_P, INT_P, INT_P, INT_P, INT_P]
lib.bridge_get_state.restype = ctypes.c_int

lib.bridge_get_graph.argtypes = [
    INT_P, INT_P, INT_P, INT_P, INT_P
]
lib.bridge_get_graph.restype = ctypes.c_int

lib.bridge_graph_has_cycle.argtypes = []
lib.bridge_graph_has_cycle.restype = ctypes.c_int

MAX_P = 10
MAX_R = 10
FLAT_SIZE = MAX_P * MAX_R


def int_array(values):
    return (ctypes.c_int * len(values))(*values)


def read_state(processes, resources):
    allocation = (ctypes.c_int * FLAT_SIZE)()
    maximum = (ctypes.c_int * FLAT_SIZE)()
    need = (ctypes.c_int * FLAT_SIZE)()
    available = (ctypes.c_int * MAX_R)()
    requests = (ctypes.c_int * FLAT_SIZE)()

    ok = lib.bridge_get_state(
        allocation, maximum, need, available, requests
    )
    if not ok:
        raise RuntimeError("Could not read simulator state")

    def matrix(flat):
        return [
            [flat[p * MAX_R + r] for r in range(resources)]
            for p in range(processes)
        ]

    return {
        "allocation": matrix(allocation),
        "maximum": matrix(maximum),
        "need": matrix(need),
        "available": [available[r] for r in range(resources)],
        "requests": matrix(requests),
    }


def safety_result():
    safe = ctypes.c_int()
    sequence = (ctypes.c_int * MAX_P)()
    length = ctypes.c_int()
    status = lib.bridge_check_safety(
        ctypes.byref(safe), sequence, ctypes.byref(length)
    )
    if status != 0:
        raise ValueError(f"Safety check failed (status {status})")
    return {
        "safe": bool(safe.value),
        "sequence": [sequence[i] for i in range(length.value)],
    }


class Handler(BaseHTTPRequestHandler):
    def send_json(self, code, data):
        payload = json.dumps(data).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(payload)))
        self.send_header("Access-Control-Allow-Origin", "http://localhost:8000")
        self.end_headers()
        self.wfile.write(payload)

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "http://localhost:8000")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.end_headers()

    def do_GET(self):
        if self.path == "/api/health":
            self.send_json(200, {
                "ok": True,
                "message": "C backend connected"
            })
        elif self.path == "/api/safety":
            try:
                if not current_processes:
                    raise ValueError("Initialize the simulator first")
                self.send_json(200, safety_result())
            except ValueError as exc:
                self.send_json(400, {"error": str(exc)})
        else:
            self.send_json(404, {"error": "Not found"})

    def do_POST(self):
        global current_processes, current_resources
        try:
            length = int(self.headers.get("Content-Length", "0"))
            if length < 1 or length > 100000:
                raise ValueError("Invalid request size")
            data = json.loads(self.rfile.read(length))
            action = data.get("action")

            if action == "init":
                p = int(data["processes"])
                r = int(data["resources"])
                if not (1 <= p <= MAX_P and 1 <= r <= MAX_R):
                    raise ValueError("Use 1–10 processes and resources")

                allocation_rows = data["allocation"]
                maximum_rows = data["maximum"]
                available_values = data["available"]

                if len(allocation_rows) != p or len(maximum_rows) != p:
                    raise ValueError("Matrix row count must match processes")
                if any(len(row) != r for row in allocation_rows + maximum_rows):
                    raise ValueError("Each matrix row must match resources")
                if len(available_values) != r:
                    raise ValueError("Available vector size must match resources")

                allocation = [0] * FLAT_SIZE
                maximum = [0] * FLAT_SIZE
                for i in range(p):
                    for j in range(r):
                        allocation[i * MAX_R + j] = int(allocation_rows[i][j])
                        maximum[i * MAX_R + j] = int(maximum_rows[i][j])

                alloc_c = int_array(allocation)
                max_c = int_array(maximum)
                avail_c = int_array([int(x) for x in available_values])
                status = lib.bridge_init(p, r, alloc_c, max_c, avail_c)
                if status != 0:
                    raise ValueError(f"Simulator rejected input (status {status})")

                current_processes, current_resources = p, r
                self.send_json(200, {
                    "state": read_state(p, r),
                    "safety": safety_result(),
                    "message": "Simulator initialized",
                })
                return

            if not current_processes:
                raise ValueError("Initialize the simulator first")

            p, r = current_processes, current_resources

            if action in ("grant", "wait"):
                process = int(data["process"])
                request_values = data["request"]
                if not (0 <= process < p) or len(request_values) != r:
                    raise ValueError("Invalid process or request dimensions")
                request = int_array([int(x) for x in request_values])

                if action == "grant":
                    safe = ctypes.c_int()
                    sequence = (ctypes.c_int * MAX_P)()
                    seq_len = ctypes.c_int()
                    status = lib.bridge_request(
                        process, request, ctypes.byref(safe),
                        sequence, ctypes.byref(seq_len)
                    )
                else:
                    status = lib.bridge_set_waiting_request(process, request)

                self.send_json(200, {
                    "status": status,
                    "state": read_state(p, r),
                    "safety": safety_result(),
                    "message": "Request processed" if status == 0 else
                               f"Request rejected (status {status})",
                })
                return

            if action == "detect":
                deadlocked = (ctypes.c_int * MAX_P)()
                count = lib.bridge_detect_deadlock(deadlocked)
                if count < 0:
                    raise ValueError("Deadlock detection failed")
                self.send_json(200, {
                    "count": count,
                    "deadlocked": [
                        i for i in range(p) if deadlocked[i]
                    ],
                    "cycle": bool(lib.bridge_graph_has_cycle()),
                    "state": read_state(p, r),
                })
                return

            if action == "recover":
                process = int(data["process"])
                if not (0 <= process < p):
                    raise ValueError("Invalid process index")
                recovered = lib.bridge_recover(process)
                if not recovered:
                    raise ValueError(
                        "Recovery failed: process may hold no resources"
                    )
                self.send_json(200, {
                    "message": f"Recovered P{process}",
                    "state": read_state(p, r),
                    "safety": safety_result(),
                })
                return

            raise ValueError("Unknown action")

        except (ValueError, KeyError, TypeError, json.JSONDecodeError) as exc:
            self.send_json(400, {"error": str(exc)})
        except Exception as exc:
            self.send_json(500, {"error": str(exc)})


current_processes = 0
current_resources = 0

if __name__ == "__main__":
    server = ThreadingHTTPServer(("127.0.0.1", 8001), Handler)
    print("C backend server running at http://127.0.0.1:8001")
    server.serve_forever()

