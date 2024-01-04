#!/bin/bash
set -euo pipefail

model=$1

cd $(dirname $0)/../../build/

rm -fr ${model} ../packages/pkg_*
../model/scripts/run.py ${model} all-release

