#ifndef PFC_SCENARIO_BUILDER_LOCATION_H
#define PFC_SCENARIO_BUILDER_LOCATION_H

#include <QAbstractItemModel>
#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QVariant>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"

namespace pfc {

//! \class Location
//!
//! \brief Setting up this class as a possible way to ahndl
//class Location : public QObject {
//  Q_OBJECT
//
//  Q_PROPERTY(QString id READ ID WRITE ID NOTIFY idChanged)
//  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
//  Q_PROPERTY(QDate date READ Date WRITE Date NOTIFY dateChanged)
//  Q_PROPERTY(QString owner READ Owner WRITE Owner NOTIFY ownerChanged)
//  Q_PROPERTY(QString affiliation READ Affiliation WRITE Affiliation NOTIFY affiliationChanged)
//  Q_PROPERTY(double latitude READ Latitude WRITE Latitude NOTIFY latitudeChanged)
//  Q_PROPERTY(double longitude READ Longitude WRITE Longitude NOTIFY longitudeChanged)
//  Q_PROPERTY(double altitude READ Altitude WRITE Altitude NOTIFY altitudeChanged)
//public:
//  Location(QObject* parent = nullptr);
//  virtual ~Location();
//  Location(const Location&) = default;
//  Location(Location&&) = default;
//  Location& operator=(const Location&) = default;
//  Location& operator=(Location&&) = default;
//
//public:
//  QString ID() const;
//  QString Name() const;
//  QDate Date() const;
//  QString Owner() const;
//  QString Affiliation() const;
//  double Latitude() const;
//  double Longitude() const;
//  double Altitude() const;
//
//public slots:
//  void ID(QString) const;
//  void Name(QString) const;
//  void Date(QDate) const;
//  void Owner(QString) const;
//  void Affiliation(QString) const;
//  void Latitude(double) const;
//  void Longitude(double) const;
//  void Altitude(double) const;
//
// singals:
//  void idChanged();
//  void nameChanged();
//  void dateChanged();
//  void ownerChanged();
//  void affiliationChanged();
//  void latitudeChanged();
//  void longitudeChanged();
//  void altitudeChanged();
//
//private:
//  QString _id;
//  QString _name;
//  QDate _date;
//  QString _owner;
//  QString _affiliation;
//  double _latitude;
//  double _longitude;
//  double _altitude;
//};

class LocationSequence : public QAbstractItemModel {
  Q_OBJECT

  Q_PROPERTY(int32_t count READ rowCount() NOTIFY countChanged)

public:
  LocationSequence(QObject* parent = nullptr);
  virtual ~LocationSequence();
  LocationSequence(xsd::cxx::tree::sequence<::msdl_1::InstallationType>&, QObject* parent = nullptr);
  LocationSequence(const LocationSequence&);
  LocationSequence(LocationSequence&&) = default;
  LocationSequence& operator=(const LocationSequence&);
  LocationSequence& operator=(LocationSequence&&) = default;

  enum LocationRoles {
    IdentityRole = Qt::UserRole + 1,
    SymbolRole,
    NameRole,
    OwnerRole,
    AffiliationRole,
    LatitudeRole,
    LongitudeRole,
    ElevationRole,
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
  void new_location();
  void drop_location(int);

  signals:
  void countChanged();
   int32_t locationAdded();
   int32_t locationDroped();
   int32_t locationModified();

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
    roles[OwnerRole] = "owner";
    roles[AffiliationRole] = "affiliation";
    roles[LatitudeRole] = "lat";
    roles[LongitudeRole] = "lon";
    roles[ElevationRole] = "alt";
    roles[DescriptionRole] = "description";

    return roles;
  }

private:
  struct Implementation;
  std::unique_ptr<Implementation> _impl;
}; //class Location
Q_DECLARE_METATYPE(pfc::LocationSequence)
} //namespace pfc
#endif //PFC_SCENARIO_BUILDER_LOCATION_H
