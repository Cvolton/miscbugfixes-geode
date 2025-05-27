#include "_Utils.hpp"

static bool s_isAmazon = false;

bool MiscBugfixes::isAmazon() {
    return s_isAmazon;
}

void MiscBugfixes::setAmazon(bool isAmazon) {
    s_isAmazon = isAmazon;
}