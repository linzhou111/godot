/*************************************************************************/
/*  godot_node_path.h                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef GODOT_NODE_PATH_H
#define GODOT_NODE_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef GODOT_CORE_API_GODOT_NODE_PATH_TYPE_DEFINED
typedef struct godot_node_path {
	uint8_t _dont_touch_that[8];
} godot_node_path;
#endif

#include "../godot.h"

void GDAPI godot_node_path_new(godot_node_path *p_np, const godot_string *p_from);
void GDAPI godot_node_path_copy(godot_node_path *p_np, const godot_node_path *p_from);

godot_string GDAPI godot_node_path_get_name(const godot_node_path *p_np, const godot_int p_idx);
godot_int GDAPI godot_node_path_get_name_count(const godot_node_path *p_np);

godot_string GDAPI godot_node_path_get_property(const godot_node_path *p_np);
godot_string GDAPI godot_node_path_get_subname(const godot_node_path *p_np, const godot_int p_idx);
godot_int GDAPI godot_node_path_get_subname_count(const godot_node_path *p_np);

godot_bool GDAPI godot_node_path_is_absolute(const godot_node_path *p_np);
godot_bool GDAPI godot_node_path_is_empty(const godot_node_path *p_np);

godot_string GDAPI godot_node_path_as_string(const godot_node_path *p_np);

void GDAPI godot_node_path_destroy(godot_node_path *p_np);

#ifdef __cplusplus
}
#endif

#endif // GODOT_NODE_PATH_H