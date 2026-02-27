// Copy of Patch/DGStompUI.js for AU build
// (see source file for comments and details)

// ============================================================
//  JAMAHA DG-Stomp  —  Amp Simulator
//  Hardware aesthetic: Yamaha DG Stomp pedal
//  Dark navy chassis · Orange LCD · Silver knobs
// ============================================================

const AMP_MODELS = [
  { id:  1, name: "LEAD 1 ", cat: "LEAD ", code: "L1 ", desc: "DG100 — AH signature lead     ", gain: 0.850, bass: 0.400, mid: 0.550, treble: 0.500, presence: 0.600, volume: 0.700, bright: false },
  { id:  2, name: "LEAD 2 ", cat: "LEAD ", code: "L2 ", desc: "DG80 — searing sustain        ", gain: 0.900, bass: 0.350, mid: 0.500, treble: 0.550, presence: 0.650, volume: 0.700, bright: false },
  { id:  3, name: "LEAD 3 ", cat: "LEAD ", code: "L3 ", desc: "Warm crunch                   ", gain: 0.750, bass: 0.550, mid: 0.600, treble: 0.450, presence: 0.500, volume: 0.750, bright: false },
  { id:  4, name: "LEAD 4 ", cat: "LEAD ", code: "L4 ", desc: "Mid-forward grind             ", gain: 0.800, bass: 0.450, mid: 0.700, treble: 0.450, presence: 0.550, volume: 0.700, bright: false },
  { id:  5, name: "LEAD 5 ", cat: "LEAD ", code: "L5 ", desc: "High gain + bright            ", gain: 0.950, bass: 0.300, mid: 0.550, treble: 0.600, presence: 0.700, volume: 0.650, bright: true },
  { id:  6, name: "LEAD 6 ", cat: "LEAD ", code: "L6 ", desc: "Scooped bright shred          ", gain: 0.800, bass: 0.350, mid: 0.450, treble: 0.700, presence: 0.750, volume: 0.700, bright: true },
  { id:  7, name: "LEAD 7 ", cat: "LEAD ", code: "L7 ", desc: "Dark compressed               ", gain: 0.700, bass: 0.500, mid: 0.650, treble: 0.400, presence: 0.450, volume: 0.800, bright: false },
  { id:  8, name: "LEAD 8 ", cat: "LEAD ", code: "L8 ", desc: "Full bass thump               ", gain: 0.850, bass: 0.550, mid: 0.700, treble: 0.350, presence: 0.400, volume: 0.750, bright: false },
  { id:  9, name: "LEAD 9 ", cat: "LEAD ", code: "L9 ", desc: "Maximum gain                  ", gain: 1.000, bass: 0.300, mid: 0.500, treble: 0.600, presence: 0.700, volume: 0.600, bright: true },
  { id: 10, name: "LEAD 10", cat: "LEAD ", code: "L10", desc: "Balanced AH tone              ", gain: 0.650, bass: 0.500, mid: 0.550, treble: 0.550, presence: 0.550, volume: 0.800, bright: false },
  { id: 11, name: "LEAD 11", cat: "LEAD ", code: "L11", desc: "Thick low-mid                 ", gain: 0.600, bass: 0.600, mid: 0.650, treble: 0.300, presence: 0.350, volume: 0.850, bright: false },
  { id: 12, name: "LEAD 12", cat: "LEAD ", code: "L12", desc: "Scooped modern bright         ", gain: 0.900, bass: 0.600, mid: 0.350, treble: 0.700, presence: 0.750, volume: 0.700, bright: true },
  { id: 13, name: "CLEAN 1", cat: "CLEAN", code: "C1 ", desc: "Fender-style clean            ", gain: 0.250, bass: 0.550, mid: 0.500, treble: 0.600, presence: 0.550, volume: 0.800, bright: false },
  { id: 14, name: "CLEAN 2", cat: "CLEAN", code: "C2 ", desc: "Warm jazz clean               ", gain: 0.200, bass: 0.650, mid: 0.550, treble: 0.450, presence: 0.450, volume: 0.850, bright: false },
  { id: 15, name: "CLEAN 3", cat: "CLEAN", code: "C3 ", desc: "Bright clean                  ", gain: 0.200, bass: 0.450, mid: 0.450, treble: 0.750, presence: 0.700, volume: 0.750, bright: true },
  { id: 16, name: "CLEAN 4", cat: "CLEAN", code: "C4 ", desc: "Slight break-up               ", gain: 0.350, bass: 0.500, mid: 0.550, treble: 0.650, presence: 0.600, volume: 0.750, bright: true },
  { id: 17, name: "CLEAN 5", cat: "CLEAN", code: "C5 ", desc: "Deep clean                    ", gain: 0.150, bass: 0.700, mid: 0.600, treble: 0.300, presence: 0.300, volume: 0.900, bright: false },
  { id: 18, name: "CLEAN 6", cat: "CLEAN", code: "C6 ", desc: "Glassy clean                  ", gain: 0.300, bass: 0.450, mid: 0.600, treble: 0.650, presence: 0.650, volume: 0.800, bright: true },
  { id: 19, name: "CLEAN 7", cat: "CLEAN", code: "C7 ", desc: "Full clean                    ", gain: 0.200, bass: 0.600, mid: 0.500, treble: 0.550, presence: 0.500, volume: 0.900, bright: false },
  { id: 20, name: "CLEAN 8", cat: "CLEAN", code: "C8 ", desc: "AH clean tone                 ", gain: 0.150, bass: 0.500, mid: 0.450, treble: 0.650, presence: 0.600, volume: 0.850, bright: true }
];

const C = {
  chassis:    '#1a1d2e',
  chassisMid: '#22263a',
  chassisDark:'#12141f',
  lcd:        '#0a1a0a',
  lcdGlow:    '#00ff44',
  lcdDim:     '#007722',
  orange:     '#ff6a00',
  orangeDim:  '#7a3200',
  amber:      '#ffaa00',
  silver:     '#c8ccd8',
  silverDark: '#888a96',
  silverLight:'#e8eaf4',
  knobBody:   '#3a3d50',
  knobRim:    '#5a5d70',
  text:       '#d0d4e8',
  textDim:    '#6a6e82',
  yellow:     '#ffe040',
  red:        '#ff3030',
  ledGreen:   '#00ff60',
  ledRed:     '#ff2020',
  border:     '#2a2e44',
};

const state = {
  model:    3,
  gain:     0.65,
  bass:     0.50,
  mid:      0.55,
  treble:   0.60,
  presence: 0.50,
  volume:   0.70,
  cabinet:  true,
  trim:     0.50,
  bright:   0,
  bypassed: false,
};

let patchConn = null;
let root      = null;

const PARAMS = {
  model:    { id: 'param1', toVal: v => Math.round(v * 19) + 1, fromVal: v => (v - 1) / 19 },
  gain:     { id: 'param2', toVal: v => v * 10,                  fromVal: v => v / 10        },
  bass:     { id: 'param3', toVal: v => v * 10,                  fromVal: v => v / 10        },
  mid:      { id: 'param4', toVal: v => v * 10,                  fromVal: v => v / 10        },
  treble:   { id: 'param5', toVal: v => v * 10,                  fromVal: v => v / 10        },
  presence: { id: 'param6', toVal: v => v * 10,                  fromVal: v => v / 10        },
  volume:   { id: 'param7', toVal: v => v * 10,                  fromVal: v => v / 10        },
  cabinet:  { id: 'param8', toVal: v => v ? 1 : 0,               fromVal: v => v >= 0.5      },
  trim:     { id: 'param9', toVal: v => v,                        fromVal: v => v             },
  bright:   { id: 'param10',toVal: v => v ? 1 : 0,               fromVal: v => v >= 0.5 ? 1 : 0 },
};

function sendParam(key, value) {
  const p = PARAMS[key];
  if (p && patchConn) patchConn.sendEventOrValue(p.id, p.toVal(value));
}

function el(tag, attrs = {}, ...children) {
  const e = document.createElement(tag);
  for (const [k, v] of Object.entries(attrs)) {
    if (k === 'style' && typeof v === 'object') Object.assign(e.style, v);
    else if (k.startsWith('on')) e.addEventListener(k.slice(2).toLowerCase(), v);
    else e.setAttribute(k, v);
  }
  for (const c of children) {
    if (c == null) continue;
    e.appendChild(typeof c === 'string' ? document.createTextNode(c) : c);
  }
  return e;
}

function svg(tag, attrs = {}, ...children) {
  const e = document.createElementNS('http://www.w3.org/2000/svg', tag);
  for (const [k, v] of Object.entries(attrs)) e.setAttribute(k, v);
  for (const c of children) if (c) e.appendChild(c);
  return e;
}

function makeKnob(opts) {
  const { label, key, min = 0, max = 1, decimals = 2, size = 64 } = opts;
  let value = state[key] ?? 0;
  let dragging = false;
  let startY = 0, startVal = 0;

  const R = size / 2;
  const knobSvg = svg('svg', { width: size, height: size, style: 'display:block;cursor:ns-resize;user-select:none' });

  function draw() {
    knobSvg.innerHTML = '';
    const norm = (value - min) / (max - min);
    const startAngle = -140;
    const endAngle   = 140;
    const angle = startAngle + norm * (endAngle - startAngle);
    const rad = (a) => (a - 90) * Math.PI / 180;

    knobSvg.appendChild(svg('circle', { cx: R, cy: R + 2, r: R - 4, fill: 'rgba(0,0,0,0.5)' }));
    knobSvg.appendChild(svg('circle', { cx: R, cy: R, r: R - 3, fill: C.knobRim }));
    const arcPath = (a1, a2, r) => {
      const x1 = R + r * Math.cos(rad(a1)), y1 = R + r * Math.sin(rad(a1));
      const x2 = R + r * Math.cos(rad(a2)), y2 = R + r * Math.sin(rad(a2));
      const large = Math.abs(a2 - a1) > 180 ? 1 : 0;
      return `M ${x1} ${y1} A ${r} ${r} 0 ${large} 1 ${x2} ${y2}`;
    };
    knobSvg.appendChild(svg('path', { d: arcPath(startAngle, endAngle, R - 5), stroke: C.chassisDark, 'stroke-width': 3, fill: 'none', 'stroke-linecap': 'round' }));
    if (norm > 0.01)
      knobSvg.appendChild(svg('path', { d: arcPath(startAngle, angle, R - 5), stroke: C.orange, 'stroke-width': 3, fill: 'none', 'stroke-linecap': 'round' }));
    knobSvg.appendChild(svg('circle', { cx: R, cy: R, r: R - 7, fill: `url(#kg${key})` }));
    const grad = svg('radialGradient', { id: `kg${key}`, cx: '35%', cy: '30%', r: '65%' });
    grad.appendChild(svg('stop', { offset: '0%', 'stop-color': C.silverLight }));
    grad.appendChild(svg('stop', { offset: '60%', 'stop-color': C.knobBody }));
    grad.appendChild(svg('stop', { offset: '100%', 'stop-color': C.chassisDark }));
    const defs = svg('defs'); defs.appendChild(grad);
    knobSvg.insertBefore(defs, knobSvg.firstChild);
    const ir = R - 10;
    const ix = R + ir * Math.cos(rad(angle));
    const iy = R + ir * Math.sin(rad(angle));
    knobSvg.appendChild(svg('circle', { cx: ix, cy: iy, r: 3, fill: C.orange }));
    knobSvg.appendChild(svg('circle', { cx: R, cy: R, r: 4, fill: C.knobBody }));
  }

  function onMouseDown(e) {
    dragging = true; startY = e.clientY; startVal = value;
    e.preventDefault();
    window.addEventListener('mousemove', onMouseMove);
    window.addEventListener('mouseup', onMouseUp);
  }
  function onMouseMove(e) {
    if (!dragging) return;
    const delta = (startY - e.clientY) / 200;
    value = Math.max(min, Math.min(max, startVal + delta * (max - min)));
    state[key] = value;
    sendParam(key, value);
    draw();
    updateDisplay();
  }
  function onMouseUp() {
    dragging = false;
    window.removeEventListener('mousemove', onMouseMove);
    window.removeEventListener('mouseup', onMouseUp);
  }

  knobSvg.addEventListener('mousedown', onMouseDown);
  knobSvg.addEventListener('dblclick', () => {
    const def = opts.defaultVal ?? ((max + min) / 2);
    value = def; state[key] = value; sendParam(key, value); draw(); updateDisplay();
  });

  draw();

  const wrap = el('div', { style: { display: 'flex', flexDirection: 'column', alignItems: 'center', gap: '4px' } },
    knobSvg,
    el('div', { style: { color: C.textDim, fontSize: '10px', letterSpacing: '0.08em', textTransform: 'uppercase', fontFamily: 'monospace' } }, label)
  );
  wrap._draw    = draw;
  wrap._key     = key;
  wrap._setValue = (v) => { value = v; draw(); };
  return wrap;
}

let lcdEl = null;
function makeLCD() {
  lcdEl = el('div', {
    style: {
      background: C.lcd,
      border: `2px solid ${C.lcdDim}`,
      borderRadius: '6px',
      padding: '10px 16px',
      boxShadow: `0 0 18px ${C.lcdGlow}22, inset 0 0 10px rgba(0,0,0,0.8)`,
      fontFamily: "'Courier New', monospace",
      minWidth: '220px',
      userSelect: 'none',
    }
  });
  updateLCD();
  return lcdEl;
}

function updateLCD() {
  if (!lcdEl) return;
  const m = AMP_MODELS[state.model - 1] || AMP_MODELS[2];
  lcdEl.innerHTML = `
    <div style="color:${C.lcdDim};font-size:9px;letter-spacing:0.15em;margin-bottom:4px">DG-STOMP  AMP MODEL</div>
    <div style="color:${C.lcdGlow};font-size:22px;font-weight:bold;letter-spacing:0.05em;text-shadow:0 0 12px ${C.lcdGlow}">${m.code}  ${m.name}</div>
    <div style="color:${C.lcdDim};font-size:10px;margin-top:4px">${m.desc}</div>
    <div style="display:flex;gap:16px;margin-top:8px;font-size:10px;color:${C.lcdGlow}88">
      <span>CAB: <span style="color:${state.cabinet ? C.lcdGlow : C.lcdDim}">${state.cabinet ? 'ON' : 'OFF'}</span></span>
      <span>BYPASS: <span style="color:${state.bypassed ? C.ledRed : C.lcdDim}">${state.bypassed ? 'ON' : 'OFF'}</span></span>
    </div>
  `;
}

let modelButtons = [];
function makeModelGrid() {
  const grid = el('div', { style: { display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: '5px' } });

  AMP_MODELS.forEach((m) => {
    const isClean = m.cat === 'CLEAN';
    const btn = el('button', {
      style: {
        background: m.id === state.model ? (isClean ? C.amber + '33' : C.orange + '33') : C.chassisDark,
        border: `1px solid ${m.id === state.model ? (isClean ? C.amber : C.orange) : C.border}`,
        borderRadius: '4px',
        color: m.id === state.model ? (isClean ? C.amber : C.orange) : C.textDim,
        fontFamily: 'monospace',
        fontSize: '11px',
        padding: '6px 4px',
        cursor: 'pointer',
        textAlign: 'center',
        transition: 'all 0.1s',
      },
      onClick: () => selectModel(m.id)
    },
      el('div', { style: { fontSize: '13px', fontWeight: 'bold' } }, m.code),
      el('div', { style: { fontSize: '9px', letterSpacing: '0.05em', marginTop: '2px', color: 'inherit' } }, m.cat)
    );
    modelButtons.push({ btn, model: m });
    grid.appendChild(btn);
  });
  return grid;
}

function selectModel(id) {
  state.model = id;
  sendParam('model', id);
  const m = AMP_MODELS[id - 1];
  if (m) {
    ['gain','bass','mid','treble','presence','volume'].forEach(k => {
      state[k] = m[k];
      sendParam(k, m[k]);
    });
    state.bright = m.bright ? 1 : 0;
    sendParam('bright', state.bright);
    updateBrightLed();
    if (root) root.querySelectorAll('[data-knob]').forEach(w => {
      if (w._setValue && state[w._key] !== undefined) w._setValue(state[w._key]);
    });
  }
  refreshModelButtons();
  updateLCD();
  updateDisplay();
}

function refreshModelButtons() {
  modelButtons.forEach(({ btn, model: m }) => {
    const active = m.id === state.model;
    const isClean = m.cat === 'CLEAN';
    btn.style.background = active ? (isClean ? C.amber + '33' : C.orange + '33') : C.chassisDark;
    btn.style.border = `1px solid ${active ? (isClean ? C.amber : C.orange) : C.border}`;
    btn.style.color = active ? (isClean ? C.amber : C.orange) : C.textDim;
  });
}

let cabLedEl = null;
function makeCabToggle() {
  cabLedEl = el('div', {
    style: {
      width: '12px', height: '12px', borderRadius: '50%',
      background: state.cabinet ? C.ledGreen : C.chassisDark,
      boxShadow: state.cabinet ? `0 0 8px ${C.ledGreen}` : 'none',
      border: `1px solid ${C.border}`,
      transition: 'all 0.15s',
    }
  });

  const btn = el('button', {
    style: {
      display: 'flex', alignItems: 'center', gap: '8px',
      background: C.chassisDark, border: `1px solid ${C.border}`,
      borderRadius: '5px', padding: '7px 12px', cursor: 'pointer',
      color: C.textDim, fontFamily: 'monospace', fontSize: '11px',
      letterSpacing: '0.1em',
    },
    onClick: () => {
      state.cabinet = !state.cabinet;
      sendParam('cabinet', state.cabinet);
      cabLedEl.style.background = state.cabinet ? C.ledGreen : C.chassisDark;
      cabLedEl.style.boxShadow  = state.cabinet ? `0 0 8px ${C.ledGreen}` : 'none';
      updateLCD();
    }
  }, cabLedEl, 'CABINET SIM');
  return btn;
}

let brightLedEl = null;
function updateBrightLed() {
  if (!brightLedEl) return;
  const on = !!state.bright;
  brightLedEl.style.background = on ? C.yellow : C.chassisDark;
  brightLedEl.style.boxShadow  = on ? `0 0 8px ${C.yellow}` : 'none';
}

function makeBrightToggle() {
  brightLedEl = el('div', {
    style: {
      width: '12px', height: '12px', borderRadius: '50%',
      background: state.bright ? C.yellow : C.chassisDark,
      boxShadow: state.bright ? `0 0 8px ${C.yellow}` : 'none',
      border: `1px solid ${C.border}`,
      transition: 'all 0.15s',
    }
  });

  const btn = el('button', {
    style: {
      display: 'flex', alignItems: 'center', gap: '8px',
      background: C.chassisDark, border: `1px solid ${C.border}`,
      borderRadius: '5px', padding: '7px 12px', cursor: 'pointer',
      color: C.textDim, fontFamily: 'monospace', fontSize: '11px',
      letterSpacing: '0.1em',
    },
    onClick: () => {
      state.bright = state.bright ? 0 : 1;
      sendParam('bright', state.bright);
      updateBrightLed();
    }
  }, brightLedEl, 'BRIGHT');
  return btn;
}

let irFilenameEl = null;
let loadCabIRFn  = null;
let resetCabIRFn = null;
let setCabHPFn   = null;
let setCabLPFn   = null;

function makeCabIRLoader() {
  irFilenameEl = el('div', {
    style: {
      color: C.orange, fontFamily: 'monospace', fontSize: '9px',
      letterSpacing: '0.06em', maxWidth: '120px',
      overflow: 'hidden', textOverflow: 'ellipsis', whiteSpace: 'nowrap',
      marginTop: '2px',
    }
  }, 'BUILT-IN V30');

  const fileInput = el('input', {
    type: 'file', accept: '.wav,.WAV',
    style: { display: 'none' },
  });
  fileInput.addEventListener('change', async (e) => {
    const file = e.target.files && e.target.files[0];
    if (!file) return;
    irFilenameEl.textContent = 'LOADING...';
    irFilenameEl.style.color = C.lcdDim;
    try {
      const arrayBuf = await file.arrayBuffer();
      const bytes    = new Uint8Array(arrayBuf);
      let b64 = '';
      const chunk = 32768;
      for (let i = 0; i < bytes.length; i += chunk) {
        b64 += String.fromCharCode(...bytes.subarray(i, Math.min(i + chunk, bytes.length)));
      }
      b64 = btoa(b64);

      if (loadCabIRFn) {
        const result = await loadCabIRFn(b64, file.name);
        if (result === 'ok') {
          const name = file.name.replace(/\.wav$/i, '').toUpperCase().slice(0, 16);
          irFilenameEl.textContent = name;
          irFilenameEl.style.color = C.orange;
        } else {
          irFilenameEl.textContent = 'ERR: ' + result;
          irFilenameEl.style.color = '#ff4444';
        }
      }
    } catch(err) {
      irFilenameEl.textContent = 'ERROR';
      irFilenameEl.style.color = '#ff4444';
    }
    fileInput.value = '';
  });

  const loadBtn = el('button', {
    style: {
      background: C.chassisDark, border: `1px solid ${C.border}`,
      borderRadius: '4px', padding: '5px 8px', cursor: 'pointer',
      color: C.textDim, fontFamily: 'monospace', fontSize: '10px',
      letterSpacing: '0.08em',
    },
    title: 'Load WAV cabinet IR',
    onClick: () => fileInput.click(),
  }, 'LOAD IR');

  const resetBtn = el('button', {
    style: {
      background: C.chassisDark, border: `1px solid ${C.border}`,
      borderRadius: '4px', padding: '5px 6px', cursor: 'pointer',
      color: C.textDim, fontFamily: 'monospace', fontSize: '10px',
      letterSpacing: '0.06em',
    },
    title: 'Reset to built-in Celestion V30 IR',
    onClick: async () => {
      if (resetCabIRFn) await resetCabIRFn();
      irFilenameEl.textContent = 'BUILT-IN V30';
      irFilenameEl.style.color = C.orange;
    },
  }, 'RST');

  function fmtHz(hz) {
    return hz >= 1000 ? (hz/1000).toFixed(1)+'k' : Math.round(hz)+'';
  }

  let hpHz = 20, lpHz = 20000;

  const hpValEl = el('div', {
    style: { color: C.lcdGlow, fontFamily: 'monospace', fontSize: '9px',
             minWidth: '32px', textAlign: 'right', letterSpacing: '0.04em' }
  }, fmtHz(hpHz));

  const lpValEl = el('div', {
    style: { color: C.lcdGlow, fontFamily: 'monospace', fontSize: '9px',
             minWidth: '32px', textAlign: 'right', letterSpacing: '0.04em' }
  }, fmtHz(lpHz));

  const sliderStyle = {
    appearance: 'none', WebkitAppearance: 'none',
    width: '90px', height: '3px', cursor: 'pointer',
    background: C.chassisDark,
    outline: 'none', borderRadius: '2px',
    accentColor: C.orange,
  };

  const HP_MIN = Math.log(20), HP_MAX = Math.log(2000);
  const hpSlider = el('input', {
    type: 'range', min: '0', max: '100', step: '1', value: '0',
    style: sliderStyle,
    title: 'Cabinet High-Pass',
  });
  hpSlider.addEventListener('input', () => {
    const t = parseInt(hpSlider.value) / 100;
    hpHz = Math.round(Math.exp(HP_MIN + t * (HP_MAX - HP_MIN)));
    hpValEl.textContent = fmtHz(hpHz);
    if (setCabHPFn) setCabHPFn(hpHz);
  });

  const LP_MIN = Math.log(1000), LP_MAX = Math.log(20000);
  const lpSlider = el('input', {
    type: 'range', min: '0', max: '100', step: '1', value: '100',
    style: sliderStyle,
    title: 'Cabinet Low-Pass',
  });
  lpSlider.addEventListener('input', () => {
    const t = parseInt(lpSlider.value) / 100;
    lpHz = Math.round(Math.exp(LP_MIN + t * (LP_MAX - LP_MIN)));
    lpValEl.textContent = fmtHz(lpHz);
    if (setCabLPFn) setCabLPFn(lpHz);
  });

  const lblStyle = { color: C.textDim, fontFamily: 'monospace', fontSize: '9px',
                     letterSpacing: '0.08em', minWidth: '18px' };

  const eqRows = el('div', {
    style: { display: 'flex', flexDirection: 'column', gap: '4px', marginTop: '4px' }
  },
    el('div', { style: { display: 'flex', alignItems: 'center', gap: '5px' } },
      el('span', { style: lblStyle }, 'HP'),
      hpSlider,
      hpValEl
    ),
    el('div', { style: { display: 'flex', alignItems: 'center', gap: '5px' } },
      el('span', { style: lblStyle }, 'LP'),
      lpSlider,
      lpValEl
    )
  );

  const wrap = el('div', {
    style: { display: 'flex', flexDirection: 'column', gap: '3px' }
  },
    el('div', { style: { display: 'flex', gap: '4px', alignItems: 'center' } },
      loadBtn, resetBtn, fileInput
    ),
    irFilenameEl,
    eqRows
  );
  return wrap;
}

let bypassLedEl = null;
function makeBypassToggle() {
  bypassLedEl = el('div', {
    style: {
      width: '12px', height: '12px', borderRadius: '50%',
      background: C.chassisDark,
      border: `1px solid ${C.border}`,
    }
  });

  const btn = el('button', {
    style: {
      display: 'flex', alignItems: 'center', gap: '8px',
      background: C.chassisDark, border: `1px solid ${C.border}`,
      borderRadius: '5px', padding: '7px 12px', cursor: 'pointer',
      color: C.textDim, fontFamily: 'monospace', fontSize: '11px',
      letterSpacing: '0.1em',
    },
    onClick: () => {
      state.bypassed = !state.bypassed;
      sendParam('volume', state.bypassed ? 0 : state.volume);
      bypassLedEl.style.background = state.bypassed ? C.ledRed : C.chassisDark;
      bypassLedEl.style.boxShadow  = state.bypassed ? `0 0 8px ${C.ledRed}` : 'none';
      updateLCD();
    }
  }, bypassLedEl, 'BYPASS');
  return btn;
}

let displayEl = null;
function makeDisplay() {
  displayEl = el('div', {
    style: {
      fontFamily: 'monospace', fontSize: '10px', color: C.textDim,
      display: 'grid', gridTemplateColumns: 'repeat(3, 1fr)', gap: '4px 12px',
      padding: '8px 12px',
      background: C.chassisDark,
      borderRadius: '5px',
      border: `1px solid ${C.border}`,
    }
  });
  updateDisplay();
  return displayEl;
}

function updateDisplay() {
  if (!displayEl) return;
  const fmt = (v) => (v * 10).toFixed(1);
  displayEl.innerHTML = `
    <span style="color:${C.textDim}">GAIN <span style="color:${C.orange}">${fmt(state.gain)}</span></span>
    <span style="color:${C.textDim}">BASS <span style="color:${C.orange}">${fmt(state.bass)}</span></span>
    <span style="color:${C.textDim}">MID <span style="color:${C.orange}">${fmt(state.mid)}</span></span>
    <span style="color:${C.textDim}">TREB <span style="color:${C.orange}">${fmt(state.treble)}</span></span>
    <span style="color:${C.textDim}">PRES <span style="color:${C.orange}">${fmt(state.presence)}</span></span>
    <span style="color:${C.textDim}">VOL <span style="color:${C.orange}">${fmt(state.volume)}</span></span>
  `;
}

function makeLogo() {
  return el('div', {
    style: {
      fontFamily: "'Arial Black', sans-serif",
      letterSpacing: '-0.02em',
      userSelect: 'none',
    }
  },
    el('span', { style: { color: C.text, fontSize: '22px', fontWeight: '900' } }, 'JAMAHA'),
    el('span', { style: { color: C.orange, fontSize: '13px', marginLeft: '8px', fontFamily: 'monospace', letterSpacing: '0.1em' } }, 'DG-STOMP'),
    el('div', { style: { color: C.textDim, fontSize: '9px', letterSpacing: '0.2em', marginTop: '1px' } }, 'AMP SIMULATOR  •  HOLDSWORTH EDITION')
  );
}

function makeScrew() {
  const s = svg('svg', { width: 16, height: 16 });
  s.appendChild(svg('circle', { cx: 8, cy: 8, r: 7, fill: C.silverDark, stroke: C.chassisDark, 'stroke-width': 1 }));
  s.appendChild(svg('line',   { x1: 4, y1: 8, x2: 12, y2: 8, stroke: C.chassisMid, 'stroke-width': 1.5, 'stroke-linecap': 'round' }));
  s.appendChild(svg('line',   { x1: 8, y1: 4, x2: 8, y2: 12, stroke: C.chassisMid, 'stroke-width': 1.5, 'stroke-linecap': 'round' }));
  return s;
}

function sectionLabel(text) {
  return el('div', {
    style: {
      color: C.textDim, fontSize: '9px', letterSpacing: '0.2em',
      textTransform: 'uppercase', fontFamily: 'monospace',
      borderBottom: `1px solid ${C.border}`,
      paddingBottom: '4px', marginBottom: '8px',
    }
  }, text);
}

export default function createPatchView(connection) {
  patchConn = connection;

  loadCabIRFn  = window._loadCabIR  || null;
  resetCabIRFn = window._resetCabIR || null;
  setCabHPFn   = window._setCabHP   || null;
  setCabLPFn   = window._setCabLP   || null;
  const getIRInfoFn = window._getIRInfo || null;

  const paramKeys = ['param1','param2','param3','param4','param5','param6','param7','param8','param9','param10'];
  const stateKeys = ['model','gain','bass','mid','treble','presence','volume','cabinet','trim','bright'];
  function requestAllParams() {
    paramKeys.forEach(pid => connection.requestParameterValue(pid));
  }
  connection.addAllParameterListener(({ endpointID, value }) => {
    const idx = paramKeys.indexOf(endpointID);
    if (idx < 0) return;
    const key = stateKeys[idx];
    const p = PARAMS[key];
    state[key] = p ? p.fromVal(value) : value;
    if (key === 'model') { refreshModelButtons(); updateLCD(); }
    if (key === 'cabinet') { if (cabLedEl) { cabLedEl.style.background = state.cabinet ? C.ledGreen : C.chassisDark; cabLedEl.style.boxShadow = state.cabinet ? `0 0 8px ${C.ledGreen}` : 'none'; } updateLCD(); }
    if (key === 'bright') { updateBrightLed(); }
    if (root) root.querySelectorAll('[data-knob]').forEach(w => { if (w._key === key) w._setValue && w._setValue(state[key]); });
    updateDisplay();
  });

  root = el('div', {
    style: {
      width: '100%', height: '100%',
      background: `linear-gradient(160deg, ${C.chassisMid} 0%, ${C.chassis} 40%, ${C.chassisDark} 100%)`,
      display: 'flex', flexDirection: 'column',
      padding: '20px 24px',
      gap: '16px',
      boxSizing: 'border-box',
      fontFamily: 'monospace',
      position: 'relative',
      overflow: 'hidden',
    }
  });

  const texture = el('div', {
    style: {
      position: 'absolute', inset: 0, pointerEvents: 'none',
      backgroundImage: 'repeating-linear-gradient(90deg, rgba(255,255,255,0.012) 0px, rgba(255,255,255,0.012) 1px, transparent 1px, transparent 4px)',
      zIndex: 0,
    }
  });
  root.appendChild(texture);

  const topBar = el('div', {
    style: { display: 'flex', justifyContent: 'space-between', alignItems: 'center', position: 'relative', zIndex: 1 }
  },
    makeScrew(),
    makeLogo(),
    makeScrew()
  );

  const lcdRow = el('div', {
    style: { display: 'flex', gap: '16px', alignItems: 'stretch', position: 'relative', zIndex: 1 }
  },
    el('div', { style: { flex: '0 0 auto' } }, makeLCD()),
    el('div', { style: { flex: 1 } },
      sectionLabel('Amp Model'),
      makeModelGrid()
    )
  );

  const knobDefs = [
    { key: 'gain',     label: 'GAIN',     defaultVal: 0.65 },
    { key: 'bass',     label: 'BASS',     defaultVal: 0.50 },
    { key: 'mid',      label: 'MID',      defaultVal: 0.55 },
    { key: 'treble',   label: 'TREBLE',   defaultVal: 0.60 },
    { key: 'presence', label: 'PRESENCE', defaultVal: 0.50 },
    { key: 'volume',   label: 'VOLUME',   defaultVal: 0.70 },
    { key: 'trim',     label: 'IN TRIM',  defaultVal: 0.50 },
  ];

  const knobsRow = el('div', {
    style: {
      display: 'flex', gap: '8px', justifyContent: 'space-between', alignItems: 'flex-end',
      padding: '14px 12px',
      background: `linear-gradient(180deg, ${C.chassisDark} 0%, ${C.chassis} 100%)`,
      borderRadius: '8px',
      border: `1px solid ${C.border}`,
      position: 'relative', zIndex: 1,
    }
  });
  knobsRow.appendChild(sectionLabel(''));

  const knobSection = el('div', { style: { position: 'relative', zIndex: 1 } });
  knobSection.appendChild(sectionLabel('Controls'));
  const knobsInner = el('div', { style: { display: 'flex', gap: '8px', justifyContent: 'space-between', alignItems: 'flex-end', padding: '12px 8px', background: `linear-gradient(180deg, ${C.chassisDark} 0%, ${C.chassis} 100%)`, borderRadius: '8px', border: `1px solid ${C.border}` } });

  knobDefs.forEach(opts => {
    const knob = makeKnob({ ...opts, size: 66 });
    knob.setAttribute('data-knob', opts.key);
    knob._key = opts.key;
    knobsInner.appendChild(knob);
  });
  knobSection.appendChild(knobsInner);

  const bottomRow = el('div', {
    style: { display: 'flex', gap: '12px', alignItems: 'center', position: 'relative', zIndex: 1 }
  },
    el('div', { style: { display: 'flex', gap: '8px', alignItems: 'flex-start' } },
      el('div', { style: { display: 'flex', flexDirection: 'column', gap: '6px' } },
        el('div', { style: { display: 'flex', gap: '8px' } },
          makeCabToggle(),
          makeBrightToggle(),
          makeBypassToggle()
        ),
        makeCabIRLoader()
      )
    ),
    makeDisplay()
  );

  const bottomBar = el('div', {
    style: { display: 'flex', justifyContent: 'space-between', alignItems: 'center', position: 'relative', zIndex: 1 }
  },
    makeScrew(),
    el('div', { style: { color: C.textDim, fontSize: '9px', letterSpacing: '0.15em' } }, 'AH HOLDSWORTH SIGNATURE EDITION'),
    makeScrew()
  );

  root.appendChild(topBar);
  root.appendChild(lcdRow);
  root.appendChild(knobSection);
  root.appendChild(bottomRow);
  root.appendChild(bottomBar);

  requestAllParams();
  if (getIRInfoFn) {
    getIRInfoFn().then(info => {
      if (info && !info.builtin && info.name && irFilenameEl) {
        const display = info.name.replace(/\.wav$/i, '').toUpperCase().slice(0, 16);
        irFilenameEl.textContent = display;
        irFilenameEl.style.color = C.orange;
      }
    }).catch(() => {});
  }
  return root;
}

