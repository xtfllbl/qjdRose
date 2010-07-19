/*
 * qjdobservationattribute.cpp
 *
 *  Created on: 2010-6-22
 *      Author: zyq
 */

#include "qjdobservationattribute.h"

int QJDObservationAttribute::length = 20;

QJDObservationAttribute::QJDObservationAttribute() {
	initialize();
}

QJDObservationAttribute::~QJDObservationAttribute() {

}

void QJDObservationAttribute::initialize() {
	idx = 0;
	cx = 0;
	cy = 0;
	offset = 0.;
	azimuth = 0.;
}
QDataStream &
operator >>(QDataStream &ds, QJDObservationAttribute &oa) {
	ds >> oa.idx >> oa.cx >> oa.cy >> oa.offset >> oa.azimuth;
	return ds;
}
QDataStream &
operator <<(QDataStream &ds, const QJDObservationAttribute &oa) {
	ds << oa.idx << oa.cx << oa.cy << oa.offset << oa.azimuth ;

	return ds;
}
