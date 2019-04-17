#ifndef PFC_SCENARIO_BUILDER_SCENARIO_H
#define PFC_SCENARIO_BUILDER_SCENARIO_H

#include <QDebug>
#include <QElapsedTimer>
#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QString>
namespace pfc {

class Scenario : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString name READ GetName WRITE SetName NOTIFY nameChanged)
  Q_PROPERTY(QString summry READ GetSummary WRITE SetSummary NOTIFY summryChanged)

public:
  Scenario() = default;

  inline QString GetName() const;
  inline void SetName(QString n);

  inline QString GetSummary() const;
  inline  void SetSummary(QString s);
signals:
  void nameChanged();
  void summryChanged();

public slots:

private:
  QString _name = "Scenario";
  QString _summry = "Whatever its a summry";

};
inline QString Scenario::GetName() const { return _name; }
inline QString Scenario::GetSummary() const { return _summry; }

inline void Scenario::SetName(QString n) { _name = n; }
inline void Scenario::SetSummary(QString s) { _summry = s; }
}

#endif //PFC_SCENARIO_BUILDER_SCENARIO_H