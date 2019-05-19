#ifndef PFC_SCENARIO_BUILDER_OBJECT_H
#define PFC_SCENARIO_BUILDER_OBJECT_H

#include <QAbstractItemModel>
#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QVariant>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"

namespace pfc {

class ObjectSequence : public QAbstractItemModel {
  Q_OBJECT

  Q_PROPERTY(int32_t count READ rowCount() NOTIFY countChanged)

public:
  ObjectSequence(QObject* parent = nullptr);
  virtual ~ObjectSequence();
  ObjectSequence(xsd::cxx::tree::sequence<::msdl_1::EquipmentItemType>&, QObject* parent = nullptr);
  ObjectSequence(const ObjectSequence&);
  ObjectSequence(ObjectSequence&&) = default;
  ObjectSequence& operator=(const ObjectSequence&);
  ObjectSequence& operator=(ObjectSequence&&) = default;

  enum ObjectRoles {
    IdentityRole = Qt::UserRole + 1,
    SymbolRole,
    NameRole,
    LatitudeRole,
    LongitudeRole,
    ElevationRole,
    OrganicRelationsRole,
    DescriptionRole,
  };

  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

public slots:
  void new_object();
  void drop_object(int);

signals:
  void countChanged();
  int32_t objectAdded();
  int32_t objectDroped();
  int32_t objectModified();

protected:
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "content";
    roles[Qt::DecorationRole] = "decoration";
    roles[Qt::EditRole] = "edit";
    roles[Qt::ToolTipRole] = "toolTip";
    roles[Qt::StatusTipRole] = "statusTip";
    roles[Qt::WhatsThisRole] = "whatsThis";
    roles[IdentityRole] = "id";
    roles[SymbolRole] = "symbol";
    roles[NameRole] = "name";
    roles[LatitudeRole] = "lat";
    roles[LongitudeRole] = "lon";
    roles[ElevationRole] = "alt";
    roles[OrganicRelationsRole] = "origin";
    roles[DescriptionRole] = "description";
    return roles;
  }

private:
  struct Implementation;
  std::unique_ptr<Implementation> _impl;

}; //class Actor
} //namespace pfc
#endif //PFC_SCENARIO_BUILDER_OBJECT_H