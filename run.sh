#!/bin/bash

for i in {1..16}; do
    (echo "hello from client $i"; cat) | nc localhost 9000 &
done

echo "100 clients started"

