
#ifndef QJDOBSERVATIONATTRIBUTE_H
#define QJDOBSERVATIONATTRIBUTE_H
#include <QDataStream>

/*********observation attribute**********/
class QJDObservationAttribute
{
public:
	static int length;
	QJDObservationAttribute();
	~QJDObservationAttribute();

	int len() const { return length; }
private:
	void initialize();
public:
	int 	idx;
	float   cx;
	float 	cy;
        float	 offset;
        float	 azimuth;
private:
	friend QDataStream & operator<< ( QDataStream & ds, const QJDObservationAttribute &s);
	friend QDataStream & operator>> ( QDataStream & ds, QJDObservationAttribute &s);
 
};

QDataStream & operator<< ( QDataStream & ds, const QJDObservationAttribute &s);
QDataStream & operator>> ( QDataStream & ds, QJDObservationAttribute &s);

#endif
