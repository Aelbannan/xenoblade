#!/usr/bin/env bash
# A/B test: 4 models × 4 TUs, all in parallel
set -euo pipefail
cd "$(dirname "$0")/.."

TUS=(
  --tu RVL_SDK/src/revolution/bte/stack/l2cap/l2c_api
  --tu CriWare/src/sofdec/mwply/mwsfdply
  --tu kyoshin/plugin/pluginUnit
  --tu kyoshin/code_800AA008
)

CONFIGS=(
  pi-harness-kimi-k2.7-code.json
  pi-harness-gpt-5.6-luna-pro.json
  pi-harness-grok-build-0.1.json
  pi-harness-glm-5.2.json
)

PIDS=()
for cfg in "${CONFIGS[@]}"; do
  model="${cfg#pi-harness-}"
  model="${model%.json}"
  echo "▶ Starting $model ..."
  npm --prefix tools/pi_harness run pi-harness -- "${TUS[@]}" --config "$cfg" \
    > "build/pi-harness/ab-test/${model}.log" 2>&1 &
  PIDS+=($!)
done

echo ""
echo "All 4 launched. PIDs: ${PIDS[*]}"
echo "Waiting..."
echo ""

FAIL=0
for i in "${!PIDS[@]}"; do
  model="${CONFIGS[$i]#pi-harness-}"
  model="${model%.json}"
  if wait "${PIDS[$i]}"; then
    echo "✅ $model finished"
  else
    echo "❌ $model failed (exit $?)"
    FAIL=1
  fi
done

echo ""
echo "Logs: build/pi-harness/ab-test/<model>.log"
echo "Ledgers: build/pi-harness/ab-test/<model>/ledger.jsonl"
exit $FAIL
