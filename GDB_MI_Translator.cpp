//
// Created by alimovdavron on 10/29/19.
//

#include "GDB_MI_Translator.h"

void GDB_MI_Translator::setBreakpoint(int line) {

}

void GDB_MI_Translator::getNBytesAt(unsigned int) {

}

void GDB_MI_Translator::getValueOf(std::string) {

}

void GDB_MI_Translator::run() {

}

void GDB_MI_Translator::next() {

}

void GDB_MI_Translator::setWatch(std::string) {

}

void GDB_MI_Translator::setBreakpoint(std::string function) {
    fprintf(pipe, "break %s", function.c_str());
}
