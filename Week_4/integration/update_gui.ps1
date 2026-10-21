$file = ".\Week_4\integration\gui\index.html"
$html = Get-Content $file -Raw

$css = @"
.demo-layout{grid-template-columns:minmax(300px,.9fr) minmax(0,1.3fr);align-items:start}
.controls-panel{background:#f8fafc}
.output-panel{min-height:650px}
.control-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:9px;margin-bottom:16px}
.config-row{display:flex;gap:12px}
.config-row .field{flex:1;min-width:0}
#graph{display:flex;flex-direction:column;gap:8px;margin-top:12px}
.edge{border:1px solid #d7deea;padding:8px 12px;border-radius:8px}
.edge.alloc{border-left:4px solid #3157d5}
.edge.req{border-left:4px solid #dc2626}
@media(max-width:850px){.demo-layout{grid-template-columns:1fr}}
"@

$html = $html.Replace('</style>', $css + "`r`n</style>")
Set-Content -Path $file -Value $html -Encoding utf8

Write-Host "GUI styling updated successfully." -ForegroundColor Green
