int main() {
    int* values = new int[2];

    values[0] = 10;
    values[1] = 20;
    values[2] = 30;  // 故意越界：合法下标只有 0、1

    delete[] values;
    return 0;
}