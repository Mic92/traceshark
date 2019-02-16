// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2018, 2019  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 * This file is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 *
 *  a) This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public
 *     License along with this library; if not, write to the Free
 *     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *     MA 02110-1301 USA
 *
 * Alternatively,
 *
 *  b) Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *     2. Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "misc/traceshark.h"
#include "ui/graphenabledialog.h"
#include "vtl/error.h"
#include "setting.h"

#include <QCheckBox>

Setting::Setting(): enabled(true), nrDep(0), nrDependents(0)
{
	bzero(dependency, sizeof(dependency));
	bzero(dependent, sizeof(dependent));
}

void Setting::setName(enum SettingIndex idx, const QString &n)
{
	settings[idx].name = n;
}

void Setting::setEnabled(enum SettingIndex idx, bool e)
{
	settings[idx].enabled = e;
}

void Setting::clearDependencies(enum SettingIndex idx)
{
	settings[idx].nrDep = 0;
}

void Setting::addDependency(enum SettingIndex idx,
			    const SettingDependency &d)
{
	SettingDependency dy;
	unsigned int *nrDependents;
	unsigned int *nrDep;

	nrDep = &settings[idx].nrDep;

	if (*nrDep >=
	    sizeof(settings[idx].dependency) / sizeof(SettingDependency))
		return;
	settings[idx].dependency[*nrDep] = d;
	(*nrDep)++;

	dy.index = idx;
	dy.desiredValue = d.desiredValue;
	nrDependents = &settings[d.index].nrDependents;
	if (*nrDependents >=
	    sizeof(settings[d.index].dependent) / sizeof(SettingDependency))
		return;
	settings[d.index].dependent[*nrDependents] = dy;
	(*nrDependents)++;
}

unsigned int Setting::getNrDependencies(enum SettingIndex idx)
{
	return settings[idx].nrDep;
}

unsigned int Setting::getNrDependents(enum SettingIndex idx)
{
	return settings[idx].nrDependents;
}

const QString &Setting::getName(enum SettingIndex idx)
{
	return settings[idx].name;
}

bool Setting::isEnabled(enum SettingIndex idx)
{
	return settings[idx].enabled;
}

const SettingDependency &Setting::getDependency(enum SettingIndex idx,
						unsigned int nr)
{
	return settings[idx].dependency[nr];
}

const SettingDependency &Setting::getDependent(enum SettingIndex idx,
					       unsigned int nr)
{
	return settings[idx].dependent[nr];
}

Setting Setting::settings[NR_SETTINGS];

int Setting::line_width = 0;

bool Setting::opengl = false;

void Setting::setLineWidth(int width)
{
	line_width = width;
}

int Setting::getLineWidth()
{
	return line_width;
}

bool Setting::isOpenGLEnabled()
{
	return opengl;
}

void Setting::setOpenGLEnabled(bool e)
{
	opengl = e;
}
