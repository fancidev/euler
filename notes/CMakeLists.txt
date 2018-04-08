cmake_minimum_required(VERSION 3.1)

##############################################################################
# Notes -- run with `make pdf`
##############################################################################

include(UseLATEX.cmake)
add_latex_document(p7.tex INPUTS euler.tex FORCE_PDF)
