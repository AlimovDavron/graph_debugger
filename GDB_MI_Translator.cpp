//
// Created by alimovdavron on 10/29/19.
//

#include "GDB_MI_Translator.h"

json GDB_MI_Translator::setTarget(std::string path) {
    return runCommand("file " + path);
}

json GDB_MI_Translator::setBreakpoint(int line) {
    return runCommand("break " + std::to_string(line));
}

json GDB_MI_Translator::getNBytesAt(unsigned int) {

}

json GDB_MI_Translator::getValueOf(std::string) {

}

json GDB_MI_Translator::run() {
    return runCommand("run");
}

json GDB_MI_Translator::next() {
    return runCommand("next");
}

json GDB_MI_Translator::setWatch(std::string variable) {
    return runCommand("watch " + variable);
}

json GDB_MI_Translator::setBreakpoint(std::string function) {
    return runCommand("break " + function);
}

json GDB_MI_Translator::gdb_continue() {
    return runCommand("c");
}
