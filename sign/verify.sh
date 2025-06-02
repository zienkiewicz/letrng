#!/bin/bash
set -e

for file in first_pub.pem second_pub.pem first.sig second.sig sum.txt message.txt sum_modified.txt; do
    if [ ! -f "$file" ]; then
        echo "Brakuje pliku: $file"
        exit 1
    fi
done

SUM=$(md5sum message.txt)
SUMPLIK=$(cat sum.txt)

if openssl dgst -sha256 -verify first_pub.pem -signature first.sig sum.txt; then
    echo "Pierwszy podpis ok"
else
    echo "Drugi podpis nie ok"
fi

# Zly klucz do podpisu
if openssl dgst -sha256 -verify second_pub.pem -signature first.sig sum.txt; then
    echo "Drugi klucz ok"
else
    echo "Drugi klucz nie ok"
fi

# Zmieniona suma
if openssl dgst -sha256 -verify first_pub.pem -signature first.sig sum_modified.txt; then
    echo "Trzeci podpis ok"
else
    echo "Trzeci podpis nie ok"
fi

[ "$SUM" == "$SUMPLIK" ] && echo "Plik message.txt ma taki sam skrot jak skrot podpisany znajdujacy sie w pliku sum.txt"

