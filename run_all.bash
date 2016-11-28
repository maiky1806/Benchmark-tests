#!/bin/bash
# This script runs all the test by group

echo 'CS 553 - Cloud Computing - Project 1'
echo '---------------------------------------------'
echo 'CPU TESTS:'
echo

for typeOper in {1..2}
do
	for numThreads in {1,2,4}
	do
		./cpu_benchmark $typeOper $numThreads
	done
done


echo
echo 'CPU LONG TESTS:'
echo

for typeOper in {1..2}
do
	./cpu_long_benchmark $typeOper
done



echo '---------------------------------------------'
echo 'MEMORY TESTS:'
echo

for typeAccess in {0,1}
do
	for blockSize in {0..2}
	do
		for numThreads in {1,2}
		do
			./memory_benchmark $blockSize $typeAccess $numThreads
		done
	done
done


echo '---------------------------------------------'
echo 'DISK TESTS:'
echo
echo 'Creating test file...'
dd if=/dev/zero of=filetest bs=1024 count=1300000 > /dev/null
dd if=/dev/zero of=filetest2 bs=1024 count=1300000 > /dev/null
echo 'Starting tests...'
echo
for IOType in {0,1}
do
	for typeAccess in {0,1}
	do
		for blockSize in {0..2}
		do
			for numThreads in {1,2}
			do
				./disk_benchmark $blockSize $IOType $typeAccess $numThreads filetest filetest2
			done
		done
	done
done

rm filetest filetest2
