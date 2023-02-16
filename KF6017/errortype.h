// Errortype.h
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

// A simple enum to store success/failure returns from functions.
// This may be expanded in future to allow for different types of FAILURE
// So be careful about using !=FAILURE to test for SUCCESS

#pragma once

enum ErrorType{SUCCESS, FAILURE};

