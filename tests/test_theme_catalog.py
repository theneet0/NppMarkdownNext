import hashlib
import importlib.util
import json
import re
import subprocess
import sys
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class ThemeCatalogTests(unittest.TestCase):
    def setUp(self):
        self.data=json.loads((ROOT/'themes/reader-themes.json').read_text(encoding='utf-8'))
        self.baseline=json.loads((ROOT/'tests/fixtures/theme-palettes.json').read_text(encoding='utf-8'))

    def test_ThemeCatalogStableIds(self):
        ids=[t['id'] for t in self.data['themes']]
        self.assertEqual(ids,self.baseline['primaryIds'])
        self.assertEqual(len(ids),77)
        self.assertEqual([t['theme'] for t in self.data['themes'][:13]],list(range(13)))
        spec=importlib.util.spec_from_file_location('reader_themes',ROOT/'tools/gen-reader-themes.py')
        generator=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(generator)
        generator.validate_catalog(self.data)
        invalid=json.loads(json.dumps(self.data))
        invalid['themes'][1]['id']=invalid['themes'][0]['id']
        with self.assertRaises(ValueError):generator.validate_catalog(invalid)

    def test_GoldMarkdownRoles(self):
        palette=(ROOT/'src/ThemePalette.inc').read_text(encoding='utf-8')
        expected={'heading':'#ffd000','bold':'#ff3d3d','italic':'#ff823f','inlineCode':'#a167ff','quote':'#e4ac73','link':'#3eb2ff','comment':'#334984'}
        gold=next(t for t in self.data['themes'] if t['id']=='bearded-v-gold-d-raynh')
        self.assertEqual(gold['markdown'],expected)
        self.assertEqual(gold['markdownOverrides'],{'bold':'#edf2f7'})
        for role,color in {**expected,**gold['markdownOverrides']}.items():
            self.assertIn('--reader-'+role+': '+color+';',palette)
        invalid=json.loads(json.dumps(self.data))
        next(t for t in invalid['themes'] if t['id']==gold['id'])['markdownOverrides']['bold']='red; color: red'
        spec=importlib.util.spec_from_file_location('reader_themes',ROOT/'tools/gen-reader-themes.py')
        generator=importlib.util.module_from_spec(spec)
        spec.loader.exec_module(generator)
        with self.assertRaises(ValueError):generator.validate_catalog(invalid)

    def test_OtherPalettesUnchanged(self):
        palette=(ROOT/'src/ThemePalette.inc').read_text(encoding='utf-8')
        raw=re.search(r'R"PALETTE_CSS\((.*?)\)PALETTE_CSS"',palette,re.S)[1]
        found={}
        for match in re.finditer(r'/\* Bearded Theme .*? \((dark|light)\) \*/\n(.*?)(?=/\* Bearded Theme |\Z)',raw,re.S):
            id=re.search(r'body\.(bearded-v-[a-z0-9-]+)\s*\{',match[2])
            if id:found[id[1]]=hashlib.sha256(match[2].strip().encode()).hexdigest()
        self.assertEqual(found,self.baseline['unchangedPalettes'])

    def test_GeneratedFilesCurrent(self):
        result=subprocess.run([sys.executable,str(ROOT/'tools/gen-reader-themes.py'),'--check'],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stdout+result.stderr)

if __name__=='__main__':unittest.main(verbosity=2)
