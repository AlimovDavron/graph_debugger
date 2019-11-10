//
// Created by alimovdavron on 10/29/19.
//

#include "GDB_MI_Translator.h"

json GDB_MI_Translator::setBreakpoint(int line) {
    return runCommand("break " + std::to_string(line));
}

json GDB_MI_Translator::getNBytesAt(unsigned int) {

}

json GDB_MI_Translator::getValueOf(std::string) {

}

json GDB_MI_Translator::run() {

}

json GDB_MI_Translator::next() {

}

json GDB_MI_Translator::setWatch(std::string) {

}

json GDB_MI_Translator::setBreakpoint(std::string) {

}
