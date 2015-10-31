#!/bin/bash
dd if=/dev/urandom of=large1.db bs=10M count=1
dd if=/dev/urandom of=large2.db bs=10M count=2
dd if=/dev/urandom of=large3.db bs=10M count=5
