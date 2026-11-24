import json
import re
from pathlib import Path

root = Path(__file__).resolve().parents[1]
bundle = (root / 'src/KelkBundle.inc').read_text(encoding='utf-8')
themes = (root / 'src/ReaderThemes.inc').read_text(encoding='utf-8')
palette = (root / 'src/ThemePalette.inc').read_text(encoding='utf-8')
shell = re.search(r'R"KELK_HTML_EOF\((.*?)\)KELK_HTML_EOF"', bundle, re.S)[1]
css = re.search(r'R"THEME_CSS\((.*?)\)THEME_CSS"', themes, re.S)[1]
script = re.search(r'R"THEME_JS\((.*?)\)THEME_JS"', themes, re.S)[1]
css = re.search(r'R"PALETTE_CSS\((.*?)\)PALETTE_CSS"', palette, re.S)[1] + css + re.search(r'R"SEMANTIC_CSS\((.*?)\)SEMANTIC_CSS"', palette, re.S)[1]
script = re.search(r'R"CATALOG_JS\((.*?)\)CATALOG_JS"', palette, re.S)[1] + script
widgets = re.search(r'R"SETTINGS_HTML\((.*?)\)SETTINGS_HTML"', themes, re.S)[1]
pos = shell.rfind('</head>')
shell = shell[:pos] + '<style>' + css + '</style><script>' + script + '</script>' + shell[pos:]
pos = shell.rfind('</body>')
shell = shell[:pos] + widgets + shell[pos:]
markdown = '''# Preview regression

1. First item.
2. Item with nested details.
   - First detail.
   - Second detail.

   Continuation after a blank line.
3. Third item.
4. Fourth item.
5. Fifth item.
6. Sixth item.
7. Seventh item.
8. Eighth item.
9. Ninth item.
   - Another nested list.
10. Tenth item.

## Code and tasks

- [ ] Unchecked task.
- [x] Checked task.

```python
def preview_is_ready():
    return True
```
'''
data = {'type': 'render', 'title': 'Preview smoke test', 'markdown': markdown, 'isDark': True, 'isBiDi': True, 'themeId': 'bearded-v-gold-d-raynh', 'caretLine': -1, 'bufferId': '11', 'renderRevision': '1'}
bridge = '''<script>
window.__initialRender=DATA;
window.__listeners=[]; window.__messages=[];
window.__state={type:'settings',sync:true,bidi:true,autoShow:true,outline:false,darkMode:-1,zoomPercent:115,fontSizePx:15,lineHeightPercent:170,maxWidthPx:860,themeId:'bearded-v-gold-d-raynh',darkModeApplicable:false};
window.__emit=function(data){window.__listeners.forEach(function(fn){fn({data:data});});};
window.chrome={webview:{addEventListener:function(type,fn){window.__listeners.push(fn);},postMessage:function(message){
 window.__messages.push(message);
 if(message==='pageLoaded') queueMicrotask(function(){window.__emit(window.__initialRender);window.__emit(window.__state);});
 if(message.indexOf('selectTheme:')===0) {
  var id=message.slice(12), entry=window.READER_THEME_CATALOG.find(function(theme){return theme.id===id;});
  if(!entry) return;
  window.__state.themeId=id;window.__state.darkModeApplicable=id==='follow';
  window.__emit({type:'setTheme',themeId:id,isDark:id==='follow'?window.__state.darkMode!==0:entry.isDark});
 }
 if(message==='toggleSync') window.__state.sync=!window.__state.sync;
 if(message==='toggleBiDi') window.__state.bidi=!window.__state.bidi;
 if(message==='toggleAutoShow') window.__state.autoShow=!window.__state.autoShow;
 if(message.indexOf('tocStateChanged:')===0) window.__state.outline=message==='tocStateChanged:true';
 if(message.indexOf('darkMode:')===0 && window.__state.darkModeApplicable) {
  window.__state.darkMode=Number(message.slice(9));
  window.__emit({type:'setTheme',themeId:'follow',isDark:window.__state.darkMode!==0});
 }
 if(message.indexOf('readingFontSize:')===0) {var n=Number(message.slice(16));if([15,17,19].includes(n))window.__state.fontSizePx=n;}
 if(message.indexOf('readingLineHeight:')===0) {var n=Number(message.slice(18));if([150,170,190].includes(n))window.__state.lineHeightPercent=n;}
 if(message.indexOf('readingWidth:')===0) {var n=Number(message.slice(13));if([680,860,1100].includes(n))window.__state.maxWidthPx=n;}
 if(message==='zoomIn') window.__state.zoomPercent+=10;
 if(message==='zoomOut') window.__state.zoomPercent-=10;
 if(message==='zoomReset') window.__state.zoomPercent=100;
 if(message.indexOf('zoomChange:')===0) window.__state.zoomPercent=Math.round(Number(message.slice(11))*100);
 if(message!=='pageLoaded') window.__emit(window.__state);
}}};
</script>'''.replace('DATA', json.dumps(data, ensure_ascii=False))
shell = shell.replace('<head>', '<head>' + bridge, 1)
output = root / 'bin' / 'reader-validation' / 'reader.html'
output.parent.mkdir(parents=True, exist_ok=True)
output.write_text(shell, encoding='utf-8')
print(output)
