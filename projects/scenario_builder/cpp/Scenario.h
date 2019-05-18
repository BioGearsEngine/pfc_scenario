#ifndef PFC_SCENARIO_BUILDER_SCENARIO_H
#define PFC_SCENARIO_BUILDER_SCENARIO_H

#include <QAbstractItemModel>
#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QVariant>
#include "Locations.h"

namespace pfc {

class Scenario : public QAbstractItemModel {
  Q_OBJECT

  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
  Q_PROPERTY(QString type READ Type WRITE Type NOTIFY typeChanged)
  Q_PROPERTY(QString version READ Version WRITE Version NOTIFY versionChanged)
  Q_PROPERTY(QDate modificationDate READ ModificationDate WRITE ModificationDate NOTIFY modificationDateChanged)
  Q_PROPERTY(QString securityClassification READ SecurityClassification WRITE SecurityClassification NOTIFY securityClassificationChanged)
  Q_PROPERTY(QString releaseRestriction READ ReleaseRestriction WRITE ReleaseRestriction NOTIFY releaseRestrictionChanged)
  Q_PROPERTY(QString purpose READ Purpose WRITE Purpose NOTIFY purposeChanged)
  Q_PROPERTY(QString description READ Description WRITE Description NOTIFY descriptionChanged)
  Q_PROPERTY(QString useLimitation READ UseLimitation WRITE UseLimitation NOTIFY useLimitationChanged)


  Q_PROPERTY(LocationSequence* locations READ Locations NOTIFY locationEvent)
public:
  Scenario(QObject* parent = nullptr);
  virtual ~Scenario();
  Scenario(std::string filename, QObject* parent = nullptr);
  Scenario(std::string path, std::string filename, QObject* parent = nullptr);
  Scenario(const Scenario&) = delete;
  Scenario(Scenario&&) = default;
  Scenario& operator=(const Scenario&) = delete;
  Scenario& operator=(Scenario&&) = default;

public:
  QString Name() const;
  QString Type() const;
  QString Version() const;
  QDate ModificationDate() const;
  QString SecurityClassification() const;
  QString ReleaseRestriction() const;
  QString Purpose() const;
  QString Description() const;
  QString UseLimitation() const;

  LocationSequence* Locations();

  enum ScenarioRoles {
    IdentityRole = Qt::UserRole + 1,

  };
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

 Qt::ItemFlags flags(const QModelIndex& index) const override;
 bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

 protected:
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "content";
    roles[Qt::DecorationRole] = "decoration";
    roles[Qt::EditRole]      = "edit";
    roles[Qt::ToolTipRole]   = "toolTip";
    roles[Qt::StatusTipRole] = "statusTip";
    roles[Qt::WhatsThisRole] = "whatsThis";
    roles[IdentityRole] = "label";

    return roles;
  }

public slots:
  void Name(QString);
  void Type(QString);
  void Version(QString);
  bool ModificationDate(QDate);
  bool ModificationDate(int, unsigned short, unsigned short);
  void SecurityClassification(QString);
  void ReleaseRestriction(QString);
  void Purpose(QString);
  void Description(QString);
  void UseLimitation(QString);

  bool marshal(QDataStream&);
  bool unmarshal(QDataStream&);
  bool Load();
  bool Load(QString);
  bool Save();
  bool SaveAs(QString filename);
signals:
  void nameChanged();
  void typeChanged();
  void versionChanged();
  void modificationDateChanged();
  void securityClassificationChanged();
  void releaseRestrictionChanged();
  void purposeChanged();
  void descriptionChanged();
  void useLimitationChanged();
  
  void locationEvent();

  private:
  struct Implementation;
  std::unique_ptr<Implementation> _impl;
};
} //namespace pfc

#endif //PFC_SCENARIO_BUILDER_SCENARIO_H