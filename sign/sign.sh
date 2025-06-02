#!/bin/bash
set -e

openssl rsa -in first.pem -pubout -out first_pub.pem
openssl rsa -in second.pem -pubout -out second_pub.pem
openssl dgst -sha256 -sign first.pem -out first.sig sum.txt
openssl dgst -sha256 -sign second.pem -out second.sig sum.txt
