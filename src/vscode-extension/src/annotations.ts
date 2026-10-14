/**
 * Reads and writes the `annotation(experiment(...))` block that the QSS Solver
 * desktop GUI stores simulation/run settings in, directly inside the .mo file
 * (see src/gui/modeleditor.cpp ModelEditor::writeAnnotations/getAnnotations).
 *
 * This is a regex-based reader/writer, not a full MicroModelica parser -- it only
 * has to recognize the fixed shape the GUI itself always produces.
 */

export interface ExperimentSettings {
  description: string;
  solver: string;
  startTime: string;
  stopTime: string;
  tolerance: string;
  absTolerance: string;
  minStep: string;
  zcHyst: string;
  derDelta: string;
  symDiff: string;
  period: string;
  parallel: string;
  partitionMethod: string;
  lps: string;
  dtSynch: string;
  dtSynchPeriod: string;
  dtStepLog: string;
  dtMin: string;
  output: string;
  outputType: string;
  scheduler: string;
  patohSettings: string;
  scotchSettings: string;
  metisSettings: string;
  jacobian: string;
  imbalance: string;
  reorderPartition: string;
  bdfPart: string;
  bdfPartitionDepth: string;
  bdfMaxStep: string;
}

export const EMPTY_SETTINGS: ExperimentSettings = {
  description: '',
  solver: 'QSS2',
  startTime: '0',
  stopTime: '1',
  tolerance: '1e-3',
  absTolerance: '1e-6',
  minStep: '',
  zcHyst: '',
  derDelta: '',
  symDiff: '',
  period: '',
  parallel: '',
  partitionMethod: '',
  lps: '',
  dtSynch: '',
  dtSynchPeriod: '',
  dtStepLog: '',
  dtMin: '',
  output: '',
  outputType: '',
  scheduler: '',
  patohSettings: '',
  scotchSettings: '',
  metisSettings: '',
  jacobian: '',
  imbalance: '',
  reorderPartition: '',
  bdfPart: '',
  bdfPartitionDepth: '',
  bdfMaxStep: '',
};

/** Tags whose value is wrapped in `{...}` by the desktop GUI (setAnnotations, modeleditor.cpp:420-433). */
const BRACED_TAGS = new Set([
  'Tolerance',
  'AbsTolerance',
  'MMO_Output',
  'MMO_Period',
  'MMO_PatohSettings',
  'MMO_ScotchSettings',
  'MMO_BDF_Part',
  'MMO_MetisSettings',
]);

/** Annotation tags the run form does not expose; read back and re-emitted unchanged (ModelEditor::searchFixedAnnot). */
export const FIXED_ANNOTATION_TAGS = ['MMO_RandomSeed', 'MMO_XOutput', 'MMO_CVODEMaxOrder'];

/** tag name -> ExperimentSettings field, in the exact order writeAnnotations() emits them. */
const TAG_ORDER: [string, keyof ExperimentSettings, boolean /* always emit even if empty */][] = [
  ['MMO_Description', 'description', true],
  ['MMO_Solver', 'solver', true],
  ['MMO_MinStep', 'minStep', false],
  ['MMO_ZCHyst', 'zcHyst', false],
  ['MMO_DerDelta', 'derDelta', false],
  ['MMO_SymDiff', 'symDiff', false],
  ['MMO_Period', 'period', false],
  ['MMO_Parallel', 'parallel', false],
  ['MMO_PartitionMethod', 'partitionMethod', false],
  ['MMO_LPS', 'lps', false],
  ['MMO_DT_Synch', 'dtSynch', false],
  ['MMO_DT_SynchPeriod', 'dtSynchPeriod', false],
  ['MMO_DT_StepLog', 'dtStepLog', false],
  ['MMO_DT_Min', 'dtMin', false],
  ['MMO_Output', 'output', false],
  ['MMO_OutputType', 'outputType', false],
  ['MMO_Scheduler', 'scheduler', false],
  ['MMO_PatohSettings', 'patohSettings', false],
  ['MMO_ScotchSettings', 'scotchSettings', false],
  ['MMO_MetisSettings', 'metisSettings', false],
  ['Jacobian', 'jacobian', false],
  ['MMO_Imbalance', 'imbalance', false],
  ['MMO_ReorderPartition', 'reorderPartition', false],
  ['MMO_BDF_Part', 'bdfPart', false],
  ['MMO_BDF_PDepth', 'bdfPartitionDepth', false],
  ['MMO_BDF_Max_Step', 'bdfMaxStep', false],
];

export function findModelName(text: string): string | undefined {
  const match = /\b(?:model|package)\s+([A-Za-z_]\w*)/.exec(text);
  return match?.[1];
}

/** Splits the inner content of `experiment(...)` on top-level commas (ignoring commas nested inside `{...}`). */
function splitTopLevel(content: string): string[] {
  const parts: string[] = [];
  let depth = 0;
  let current = '';
  for (const ch of content) {
    if (ch === '{') depth++;
    if (ch === '}') depth--;
    if (ch === ',' && depth === 0) {
      parts.push(current);
      current = '';
    } else {
      current += ch;
    }
  }
  if (current.trim().length) {
    parts.push(current);
  }
  return parts;
}

function unwrapValue(raw: string): string {
  const value = raw.trim();
  if (value.startsWith('{') && value.endsWith('}')) {
    return value.slice(1, -1);
  }
  return value;
}

function experimentBlockRegex(): RegExp {
  return /annotation\s*\(\s*experiment\s*\(([\s\S]*?)\)\s*\)\s*;/;
}

/** Reads the current experiment(...) annotation values, if any, from a .mo file's text. */
export function readAnnotations(text: string): { values: Partial<ExperimentSettings>; fixed: Record<string, string> } {
  const match = experimentBlockRegex().exec(text);
  const values: Partial<ExperimentSettings> = {};
  const fixed: Record<string, string> = {};
  if (!match) {
    return { values, fixed };
  }
  const byTag = new Map<string, string>();
  for (const part of splitTopLevel(match[1])) {
    const eq = part.indexOf('=');
    if (eq < 0) continue;
    const tag = part.slice(0, eq).trim();
    const value = unwrapValue(part.slice(eq + 1));
    byTag.set(tag, value);
  }
  for (const [tag, field] of TAG_ORDER) {
    if (byTag.has(tag)) {
      values[field] = byTag.get(tag);
    }
  }
  // MMO_Description is stored quoted ("text"); ModelEditor::description() always strips the
  // quotes for display/editing and setDescription() always re-adds them (modeleditor.hpp:48,70-74).
  if (values.description !== undefined) {
    values.description = values.description.replace(/"/g, '');
  }
  if (byTag.has('StartTime')) values.startTime = byTag.get('StartTime');
  if (byTag.has('StopTime')) values.stopTime = byTag.get('StopTime');
  if (byTag.has('Tolerance')) values.tolerance = byTag.get('Tolerance');
  if (byTag.has('AbsTolerance')) values.absTolerance = byTag.get('AbsTolerance');
  for (const tag of FIXED_ANNOTATION_TAGS) {
    if (byTag.has(tag)) {
      fixed[tag] = byTag.get(tag)!;
    }
  }
  return { values, fixed };
}

function formatLine(tag: string, value: string, trailingComma: boolean): string {
  let wrapped = value;
  if (tag === 'MMO_Description') {
    wrapped = `"${value.replace(/"/g, '')}"`;
  } else if (BRACED_TAGS.has(tag)) {
    wrapped = `{${value}}`;
  }
  return `\t\t${tag}=${wrapped}${trailingComma ? ',' : ''}`;
}

/**
 * Regenerates the experiment(...) annotation block for `settings` (in the same tag
 * order as the desktop GUI) and inserts/replaces it directly before `end <name>;`.
 */
export function writeAnnotations(
  text: string,
  modelName: string,
  settings: ExperimentSettings,
  fixed: Record<string, string> = {}
): string {
  const lines: string[] = ['\tannotation(', '\texperiment('];
  for (const [tag, field, always] of TAG_ORDER) {
    const value = settings[field];
    if (always || (value && value.length)) {
      lines.push(formatLine(tag, value ?? '', true));
    }
  }
  for (const tag of FIXED_ANNOTATION_TAGS) {
    if (fixed[tag]) {
      lines.push(formatLine(tag, fixed[tag], true));
    }
  }
  lines.push(formatLine('StartTime', settings.startTime, true));
  lines.push(formatLine('StopTime', settings.stopTime, true));
  lines.push(formatLine('Tolerance', settings.tolerance, true));
  lines.push(formatLine('AbsTolerance', settings.absTolerance, false));
  lines.push('\t));');
  const block = lines.join('\n');

  const endStatement = new RegExp(`end\\s+${modelName}\\s*;`);
  const endMatch = endStatement.exec(text);
  if (!endMatch) {
    throw new Error(`Could not find "end ${modelName};" in the model file.`);
  }

  const before = text.slice(0, endMatch.index);
  const after = text.slice(endMatch.index);

  const existing = experimentBlockRegex().exec(before);
  const head = existing ? before.slice(0, existing.index) : before;

  return `${head.replace(/\s*$/, '')}\n${block}\n${after}`;
}
