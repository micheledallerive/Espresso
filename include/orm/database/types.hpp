#pragma once

namespace espresso::orm {

enum class SQLType {
    BOOLEAN,
    BIGINT,
    CHAR,
    DATE,
    FLOAT,
    INTEGER,
    TEXT,
};

using DB = EspressoSettings::DB;

}