//
// Created by daddabarba on 25/02/19.
//

#ifndef RLIB_DEFS_H
#define RLIB_DEFS_H

#pragma once

#define RLIB_NS rlib
#define RLIB_BASES_NS rlib_bases
#define RLIB_ADDONS_NS rlib_addons

#define RLIB_START namespace RLIB_NS {
#define RLIB_END }
#define RLIB ::RLIB_NS

#define RLIB_BASES_START RLIB_START namespace RLIB_BASES_NS {
#define RLIB_BASES_END } RLIB_END
#define RLIB_BASES RLIB::RLIB_BASES_NS

#define RLIB_ADDONS_START RLIB_START namespace RLIB_ADDONS_NS {
#define RLIB_ADDONS_END } RLIB_END
#define RLIB_ADDONS RLIB::RLIB_ADDONS_NS

typedef unsigned int Action;
typedef unsigned int State;
typedef double Reward;

typedef unsigned int Space_Size;

#endif //RLIB_DEFS_H