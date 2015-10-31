#!/bin/sh
for i in {1..5001}; do
	dd if=/dev/urandom of=small$i.db bs=512 count=1 2> /dev/null
done
