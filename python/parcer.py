import csv
import matplotlib.pyplot as plt
import sys

def parse(path: str = "output.csv"):
    """
    Функция читает поля time и size из файла и возвращает метрики трафика.
    args:
        path: Путь до файла
    returns:
        intervals:  Массив интервалов между пакетами (сек)
        sizes:      Массив размеров пакетов (биты)
        name:       Название модели (equal/poisson)
        avg_size:   Средний размер пакета (биты)
        avg_bitrate:Средний битрейт (бит/с)
        avg_delay:  Средняя задержка между пакетами (сек)
    """
    times = []
    sizes = []
    with open(path, 'r', newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            times.append(float(row[0]))
            sizes.append(int(row[1]))
    if len(times) < 1:
        return [], [], "equal", 0, 0.0, 0.0
    elif len(times) == 1:
        avg_size = sum(sizes) / len(sizes)
        avg_bitrate = sum(sizes) / max(times)
        return [], [], "equal", avg_size, avg_bitrate, 0.0
    else:
        intervals = [times[i] - times[i-1] for i in range(1, len(times))]
        if max(intervals) - min(intervals) < 1e-9:
            name = "equal"
        else:
            name = "poisson"
        avg_size = sum(sizes) / len(sizes)
        avg_bitrate = sum(sizes) / max(times)
        avg_delay = sum(intervals) / len(intervals)
    return intervals, sizes, name, avg_size, avg_bitrate, avg_delay


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise ValueError("Использование: python parcer.py <путь_до_csv>")
    intervals, sizes, name, avg_size, avg_bitrate, avg_delay = parse(sys.argv[1])
    print(f"Модель трафика: {name}")
    print(f"Средний размер пакета: {avg_size:.4f} байт")
    print(f"Средний битрейт: {avg_bitrate:.4f} бит/с")
    print(f"Средняя задержка: {avg_delay:.4f} с")
    if intervals:
        plt.figure()
        plt.hist(intervals, bins=50, color='skyblue', edgecolor='black')
        plt.title('Гистограмма задержек между пакетами')
        plt.xlabel('Задержка (сек)')
        plt.ylabel('Количество')
        plt.tight_layout()
        plt.show()
    else:
        print("Нет данных для гистограммы задержек")
    if sizes:
        plt.figure()
        plt.hist(sizes, bins=50, color='skyblue', edgecolor='black')
        plt.title('Гистограмма размеров пакетов')
        plt.xlabel('Размер пакета (байт)')
        plt.ylabel('Количество')
        plt.tight_layout()
        plt.show()
    else:
        print("Нет данных для гистограммы размеров")
    
