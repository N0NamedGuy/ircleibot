#!/bin/bash

function escape_squote {
    echo "$1" | sed -e "s/'/ /g";
}

function escape_dquote {
    echo "$1" | sed -e "s/\"/\\\"";
}

echo `escape_squote "meh'''\"'" `
