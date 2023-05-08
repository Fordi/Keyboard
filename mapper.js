const fs = require('fs');
const path = require('path');

const usage = (err = null) => {
  console.log('Matrix scan decoder');
  if (err) {
    console.error(`!!! ${err}`);
  }
  console.log(`  Usage: node ${require('path').basename(process.argv[1])} [scancodes.txt]`);
  console.log(`  Generates a config.h from a scancode list`);
  console.log(`  Scancode lines should be a tab-delimited file with columns`);
  console.log(`    KEY_NAME	[FN]	PIN1	PIN2`);
  console.log(`    'FN' is optional and, if present, marks the key as alternate, triggered by the Fn key`);
  console.log(`  Keys without pins are ignored`);
  console.log(`  Lines starting with '#' are ignored`);
  console.log(`You may switch between Teensy3.2 and TeensyLC by adding  the type as a line.`);
  process.exit(-1);
};

// Start entering keys below:
if (process.argv.length !== 3) usage();

if (!fs.existsSync(process.argv[2])) {
  usage(`Scancode file ${process.argv[2]} does not exist`);
}

const keyList = fs.readFileSync(process.argv[2], { encoding: 'utf-8' });

// Pin wirings for Frank Adams' matrix decoder boards.
const PINS = {
  TEENSYLC: [23, 0, 22, 1, 24, 2, 21, 3, 25, 4, 20, 5, 19, 6, 18, 7, 17, 8, 16, 9, 15, 10, 14, 11, 26, 12],
  TEENSY32: [23, 0, 22, 1, 21, 2, 20, 3, 19, 4, 18, 5, 17, 6, 24, 7, 25, 8, 33, 9, 26, 10, 27, 11, 28, 12, 32, 31, 30, 29, 16, 15, 14, 13],
  TEENSY40: [23, 0, 22, 1, 21, 2, 20, 3, 19, 4, 18, 5, 17, 6, 29, 7, 31, 8, 33, 9, 32, 10, 30, 11, 28, 12, 27, 26, 25, 24, 16, 15, 14, 13],
};

let teensyPins = PINS.TEENSYLC;

const buffer = [];
// Create a hash where each connection is doubly-linked, 
// e.g., KEY 1 2 will have a { '1': { '2': key }, '2': { '1': key } }
const keyMap = keyList.split('\n').reduce((matrix, l) => {
  const line = l.replace(/#.*$/, '').trim().toUpperCase();
  if (!line.length) return matrix;
  if (PINS[line]) {
    console.log(`Using ${line} board`);
    teensyPins = PINS[line];
    return matrix;
  }
  const [name, ...rest] = l.split(/[\t\s]+/);
  const key = { name };
  if (rest[0] === 'FN') {
    key.fn = true;
    rest.shift();
  }
  const [pin1, pin2] = rest.map(a => parseInt(a)).sort();
  if (isNaN(pin1) || isNaN(pin2)) {
    return matrix;
  }
  matrix[pin1] = { ...matrix[pin1], [pin2]: key };
  matrix[pin2] = { ...matrix[pin2], [pin1]: key };
  return matrix;
}, {});

// Sort by longest sets; we'll assume the longest to be the rows, and eliminate duplicates along the way
const maxKeys = Object.keys(keyMap).sort((a, b) => Object.keys(b).length - Object.keys(a).length);
for (let i = 0; i < maxKeys.length; i++) {
  const row = maxKeys[i];
  Object.keys(keyMap[maxKeys[i]] || {}).forEach((col) => {
    delete keyMap[col];
  });
}

// Get all the "row" pins
const inputs = Object.keys(keyMap).sort((a, b) => a - b);
const outputs = [...new Set(Object.keys(keyMap).reduce((outs, row) => 
  [...outs, ...Object.keys(keyMap[row])]
, []))].sort((a, b) => a - b);

console.log("Row pins: ", inputs.join(', '));
console.log("Column pins: ", outputs.join(', '));

if (!inputs.length || !outputs.length) {
  usage(`Please fill out ${process.argv[2]} before running this script`);
}

let open = [];
let last = -1;
const allPins = [...new Set([...inputs, ...outputs])].sort((a, b) => a - b);

allPins.forEach(pin => {
    const p  = parseInt(pin);
    if (p !== 1 + last && last !== -1) {
      open.push(last + 1);
    }
    last = p;
});

if (open.length) {
  console.warn(`WARNING: The following FPC pins appear to be unconnected: ${open.join(', ')}`);
  console.warn('Was everything wired up properly?');
}

buffer.push(`#ifndef KEYMAP_H\n#define KEYMAP_H`);
buffer.push(`#include <keylayouts.h>`);
buffer.push(`#define MODIFIERKEY_FN 0x8f`);
buffer.push(`#define MATRIX_ROWS ${inputs.length}`);
buffer.push(`#define MATRIX_COLS ${outputs.length}`);

buffer.push(
  'int normal[MATRIX_ROWS][MATRIX_COLS] = {\n\t' +
  inputs.map(low => {
    return '{ ' + outputs.map(high => {
      const key = keyMap[low][high];
      if (key && !key.fn && !key.name.startsWith('MODIFIER')) {
        return key.name;
      }
      return '0';
    }).join(', ') + ' }';
  }).join(',\n\t') +
  '\n};'
);

buffer.push(
    'int modifier[MATRIX_ROWS][MATRIX_COLS] = {\n\t' +
    inputs.map(low => {
       return '{ ' + outputs.map(high => {
           const key = keyMap[low][high];
           if (key && !key.fn && key.name.startsWith('MODIFIER')) {
             return key.name;
           }
           return '0';
        }).join(', ') + ' }';
    }).join(',\n\t') +
    '\n};'
);

buffer.push(
    'int media[MATRIX_ROWS][MATRIX_COLS] = {\n\t' +
    inputs.map(low => {
       return '{ ' + outputs.map(high => {
           const key = keyMap[low][high];
           if (key && key.fn) return key.name;
           return '0';
        }).join(', ') + ' }';
    }).join(',\n\t') +
    '\n};'
);

buffer.push(`int Row_IO[MATRIX_ROWS] = { ${inputs.map(pin => teensyPins[pin - 1]).join(', ')} };`);
buffer.push(`int Col_IO[MATRIX_COLS] = { ${outputs.map(pin => teensyPins[pin - 1]).join(', ')} };`);
buffer.push(`#endif`);
const KEYMAP = path.join(__dirname, 'keymap.h');
fs.writeFileSync(KEYMAP, buffer.join('\n\n') + '\n', { encoding: 'utf-8' });
console.log(`Wrote config for ${inputs.length}x${outputs.length} matrix to ${KEYMAP}`);
