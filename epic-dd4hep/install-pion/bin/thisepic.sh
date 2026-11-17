#!/bin/sh

export DETECTOR=epic
export DETECTOR_PATH=/home/devesh/eic/epic-dd4hep/install-pion/share/epic
export DETECTOR_CONFIG=${1:-epic}
export DETECTOR_VERSION=26.02.0-25-ga4202cc0d-dirty

## Warn is not the right name (this script is sourced, hence $1)
if [[ "$(basename ${BASH_SOURCE[0]})" != "thisepic.sh" ]]; then
        echo "Warning: This script will cease to exist at '$(realpath --no-symlinks ${BASH_SOURCE[0]})'."
        echo "         Please use the version at '$(realpath --no-symlinks $(dirname ${BASH_SOURCE[0]})/bin/thisepic.sh)'."
fi

## Export detector libraries
if [[ "$(uname -s)" = "Darwin" ]] || [[ "$OSTYPE" == "darwin"* ]]; then
        export DYLD_LIBRARY_PATH="/home/devesh/eic/epic-dd4hep/install-pion/lib${DYLD_LIBRARY_PATH:+:$DYLD_LIBRARY_PATH}"
else
        export LD_LIBRARY_PATH="/home/devesh/eic/epic-dd4hep/install-pion/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi
