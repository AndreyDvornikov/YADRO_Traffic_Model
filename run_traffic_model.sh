set -euo pipefail

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
TRAFFIC_MODEL="$SCRIPT_DIR/cpp/build/traffic_model"
PARCER="$SCRIPT_DIR/python/parcer.py"

CONFIGS="$1"
if [ ! -f "$CONFIGS" ]; then
    echo "Использование: $0 <config_file> [output_csv]" >&2
    exit 1
fi
CSV="${2:-output.csv}"

if [ ! -f "$TRAFFIC_MODEL" ]; then
    echo "$TRAFFIC_MODEL не найден! Необходимо собрать проект" >&2
    exit 2
fi
if [ ! -f "$PARCER" ]; then
    echo "$PARCER не найден!" >&2
    exit 3
fi

echo "Генерация пакетов..."
"$TRAFFIC_MODEL" "$CONFIGS" "$CSV"
if [ $? -ne 0 ]; then
    echo "Ошибка при генерации трафика" >&2
    exit 4
fi

if ! command -v python3 &> /dev/null; then
    echo "Python3 не найден" >&2
    exit 5
fi

if ! python3 -c "import matplotlib" &> /dev/null; then
    echo "Matplotlib не установлен. Установите: pip install matplotlib" >&2
    exit 6
fi

echo "Запуск анализа CSV..."
python3 "$PARCER" "$CSV"
if [ $? -ne 0 ]; then
    echo "Ошибка при анализе CSV" >&2
    exit 5
fi