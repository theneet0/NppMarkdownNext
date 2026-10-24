// Generates src/BeardedVariants.inc (CSS for every Bearded theme variation + JS list)
// from the official BeardedBear/bearded-theme Zed palette export.
//
// Usage: node tools/gen-bearded.mjs
//
// Output:
//   s_beardedVariantsCss  - appended to the preview <style> block
//   s_beardedVariantsJs   - injected as window.BEARDED_VARIANTS before </body>
//
// Class naming: default variation -> body.bearded,
// any other variation -> body.bearded-v-<id> (id is kebab-case of the label).

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const SRC = 'C:/Users/User/AppData/Local/Temp/opencode/bearded-theme/dist/zed/themes/bearded-theme.json';
const __dirname = path.dirname(fileURLToPath(import.meta.url));
const OUT = path.join(__dirname, '..', 'src', 'BeardedVariants.inc');

// ---------- color helpers ----------
function parseHex(h) {
  if (typeof h !== 'string') return null;
  let s = h.trim().replace('#', '').toLowerCase();
  if (s.length === 3) s = s.split('').map((c) => c + c).join('');
  if (s.length === 6) s += 'ff';
  if (s.length !== 8 || /[^0-9a-f]/.test(s)) return null;
  return [
    parseInt(s.slice(0, 2), 16),
    parseInt(s.slice(2, 4), 16),
    parseInt(s.slice(4, 6), 16),
    parseInt(s.slice(6, 8), 16),
  ];
}
function toHex6(r, g, b) {
  const cl = (v) => Math.max(0, Math.min(255, Math.round(v))).toString(16).padStart(2, '0');
  return '#' + cl(r) + cl(g) + cl(b);
}
function blend(a, b, t) {
  const A = parseHex(a), B = parseHex(b);
  if (!A || !B) return a || b || '#888888';
  return toHex6(A[0] * t + B[0] * (1 - t), A[1] * t + B[1] * (1 - t), A[2] * t + B[2] * (1 - t));
}
function alpha8(h, a2) {
  const p = parseHex(h);
  return p ? toHex6(p[0], p[1], p[2]) + a2 : (h || '#888888');
}
function rgba(h, a) {
  const p = parseHex(h) || [136, 136, 136, 255];
  return `rgba(${p[0]}, ${p[1]}, ${p[2]}, ${a})`;
}
function luminance(h) {
  const p = parseHex(h);
  if (!p) return 128;
  return 0.2126 * p[0] + 0.7152 * p[1] + 0.0722 * p[2];
}
function pick(...cands) {
  for (const c of cands) if (c && c !== 'transparent') return c;
  return null;
}

// ---------- load palette ----------
const raw = JSON.parse(fs.readFileSync(SRC, 'utf8'));
const themes = raw.themes;
if (!Array.isArray(themes) || themes.length === 0) {
  console.error('No themes found in', SRC);
  process.exit(1);
}

const seenIds = new Set();
const variants = []; // { id, label } - non-default variations only
let cssOut = '';

// Emit one complete (self-contained) palette block for a theme under the given selector.
function emitBlock(t, selector) {
  const name = t.name || '';
  const st = t.style || {};

  // ----- palette extraction -----
  const bg = pick(st['editor.background'], st.background, '#ffffff');
  const text = pick(st['editor.foreground'], st.foreground, st.text, '#000000');
  const dark = luminance(bg) < 140;
  const muted = pick(st['text.muted'], blend(text, bg, 0.6));
  const border = pick(st.border, blend(text, bg, 0.8));
  const surface = pick(st['surface.background'], blend(bg, text, dark ? 0.05 : 0.04));
  const elevated = pick(st['elevated_surface.background'], blend(bg, text, dark ? 0.1 : 0.08));
  const panel = pick(st['panel.background'], bg);
  const accent = pick(st['border.focused'], st['text.accent'], st.modified, text);
  const link = pick(st.modified, st.info, accent);

  const cNote = pick(st.info, accent);
  const cTip = pick(st.success, '#2ea043');
  const cImportant = pick(st.renamed, '#a371f7');
  const cWarning = pick(st.warning, '#d29922');
  const cCaution = pick(st.error, '#d1242f');

  const syn = st.syntax || {};
  const synColor = (...keys) => {
    for (const k of keys) {
      const v = syn[k];
      if (v && typeof v.color === 'string' && v.color !== 'transparent') return v.color;
    }
    return text;
  };
  const hlKeyword = synColor('keyword', 'keyword.control', 'keyword.import');
  const hlType = synColor('class', 'constructor', 'type');
  const hlString = synColor('string', 'string.special');
  const hlComment = synColor('comment', 'comment.doc');
  const hlNumber = synColor('constant.numeric', 'constant', 'constant.builtin');
  const hlPreproc = synColor('preprocessor', 'function.macro', 'constant.character');
  const hlOperator = synColor('operator', 'keyword.operator');

  const V = [
    ['--bg-page', bg],
    ['--text-primary', text],
    ['--text-secondary', muted],
    ['--border-color', border],
    ['--code-bg', surface],
    ['--code-header-bg', elevated],
    ['--code-border', border],
    ['--quote-bg', surface],
    ['--quote-bar', accent],
    ['--table-alt', surface],
    ['--table-header', elevated],
    ['--link-color', link],
    ['--toolbar-bg', rgba(panel, 0.94)],
    ['--toolbar-border', border],
    ['--toolbar-shadow', dark ? '0 4px 20px rgba(0, 0, 0, 0.45)' : '0 4px 20px rgba(0, 0, 0, 0.18)'],
    ['--search-match', pick(st['search.match_background'], elevated)],
    ['--search-active', accent],
    ['--alert-note-bg', pick(st['info.background'], alpha8(cNote, '26'))],
    ['--alert-note-bar', cNote],
    ['--alert-tip-bg', pick(st['success.background'], alpha8(cTip, '26'))],
    ['--alert-tip-bar', cTip],
    ['--alert-important-bg', pick(alpha8(cImportant, '26'))],
    ['--alert-important-bar', cImportant],
    ['--alert-warning-bg', pick(st['warning.background'], alpha8(cWarning, '26'))],
    ['--alert-warning-bar', cWarning],
    ['--alert-caution-bg', pick(st['error.background'], alpha8(cCaution, '26'))],
    ['--alert-caution-bar', cCaution],
    ['--drawer-bg', panel],
  ];

  let out = `/* ${name} (${t.appearance || '?'}) */\n`;
  out += `${selector} {\n`;
  for (const [k, v] of V) out += `    ${k}: ${v};\n`;
  out += '}\n';
  out += `${selector} .hl-keyword { color: ${hlKeyword}; }\n`;
  out += `${selector} .hl-type { color: ${hlType}; }\n`;
  out += `${selector} .hl-string { color: ${hlString}; }\n`;
  out += `${selector} .hl-comment { color: ${hlComment}; font-style: italic; }\n`;
  out += `${selector} .hl-number { color: ${hlNumber}; }\n`;
  out += `${selector} .hl-preprocessor { color: ${hlPreproc}; }\n`;
  out += `${selector} .hl-operator { color: ${hlOperator}; }\n`;
  out += '\n';
  return out;
}

// The pack ships no plain "Bearded Theme" entry; the canonical base variation is
// the first registry entry (Arc) - map the bare `body.bearded` class to it.
cssOut += emitBlock(themes[0], 'body.bearded');

for (const t of themes) {
  const name = t.name || '';
  const label = name.replace(/^Bearded Theme\s*/, '').trim();
  if (label === '') continue;

  const id = label
    .toLowerCase()
    .replace(/\(experimental\)/g, '')
    .replace(/[^a-z0-9]+/g, '-')
    .replace(/^-+|-+$/g, '')
    .replace(/^feat-/, '');
  if (!id) {
    console.error('Empty id for theme:', name);
    process.exit(1);
  }
  if (seenIds.has(id)) {
    console.error('Duplicate id:', id, 'from', name);
    process.exit(1);
  }
  seenIds.add(id);
  variants.push({ id: 'bearded-v-' + id, label });
  cssOut += emitBlock(t, 'body.bearded-v-' + id);
}

// Pin the user's favorite variation first in the submenu list.
const FAVORITE = 'gold-d-raynh';
variants.sort((a, b) => {
  const fa = a.id === 'bearded-v-' + FAVORITE ? 0 : 1;
  const fb = b.id === 'bearded-v-' + FAVORITE ? 0 : 1;
  if (fa !== fb) return fa - fb;
  return 0;
});

const jsList = '[' + variants.map((v) => JSON.stringify(v)).join(',') + ']';

const out = `// AUTO-GENERATED by tools/gen-bearded.mjs - do not edit by hand.
// Source: BeardedBear/bearded-theme (dist/zed/themes/bearded-theme.json), ${themes.length} variations.

const char* const s_beardedVariantsCss =
R"CSS(
${cssOut})CSS"
;

const char* const s_beardedVariantsJs =
R"JS(${jsList})JS"
;
`;

fs.writeFileSync(OUT, out, 'utf8');
console.log(`Wrote ${OUT}`);
console.log(`  CSS bytes: ${cssOut.length}, variations: ${variants.length + 1} (incl. default)`);
