# Stock implementation in C
To build the sources execute following commands:
```bash
mkdir build
cd build/
cmake ..
make -j2
make test
test/StockTest_tests
```

To try the execution of the test data:
```bash
cd build/
./stock -i /tmp/input.data -o /tmp/output.data
```
