/*
 * Copyright © 2022 Jayden Chan. All rights reserved.
 *
 * path_tracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * path_tracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with path_tracer. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PROGRESS_H
#define PROGRESS_H

#include <iostream>

namespace path_tracer {

inline void print_progress(const float pct) {
  const auto percent = std::min(pct * 100.0f, 100.0f);
  std::cout << " progress: " << percent << "%     \r" << std::flush;
}

} // namespace path_tracer

#endif // PROGRESS_H
