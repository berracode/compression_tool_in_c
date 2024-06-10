# Compression tool
This tool written in C is a simple compression tool inspired in https://codingchallenges.fyi/challenges/challenge-huffman/




## Run program

```bash
./cpress compress ./test_files/victor.txt
```

## Valgrind command

```sh
valgrind --leak-check=full --track-origins=yes ./cpress compress ./test_files/victor.txt
```
