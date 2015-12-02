create database `BookTickets`;

use `BookTickets`;

create table `Area`
(
	AreaId		char(3)		not null primary key,
    AreaName	varchar(20)	not null
);

create table Info
(
	Id 		char(14)		not null primary key,
    Name	varchar(20)		not null,
    Sex		bit				not null,
    Proof	varchar(20)		not null,
    Which	bit				not null,
    Oranization	varchar(50)
);

create table Passinfo
(
	Id 			char(14)		not null primary key,
    Nickname	varchar(20)		not null,
    Password	varchar(20)		not	null default "000000",
    Email		varchar(50)		not	null,
    Remark		varchar(100)
);

create table Flight
(
	Num			char(3)			not null primary key,
    SouId		char(3)			not null,
    DesId		char(3)			not null,
    SetOff		datetime		not null,
    Arrive		datetime		not null,
    TotNum		int				not null,
    Business	int				not null,
    Price		decimal(6, 2)	not null,
    BusPrice	decimal(6, 2)	not null,
    Discount	float(2)		not null default 1,
    Cancel		bit				not null,
	constraint	chk_Person		check (Discount > 0 and Discount <= 1)
);

create table Tickets
(
	TicketsId	char(16)		not null primary key,
    Id			char(14)		not null,
    Clear		bit				not null
);

create table RealNum
(
	Num			char(3)			not null primary key,
    Business	int				not null,
    Normal		int				not null
);

create table GiveUp
(
	Num			char(3)			not null primary key,
    Pos			int				not null
);

create table Agency
(
	AgencyId	char(6)			not null primary key,
    CompanyName	varchar(50)		not null,
    Remark		varchar(200)	null
);

create table Route
(
	AgencyId	char(6)			not null primary key,
    Num			char(3)			not null,
    TotalNum	int				not null,
    Price		decimal(6, 2)	null
);

alter table `Route` add unique(`Num`);

create table Company
(
	CompanyId	char(6)			not null primary key,
    CompanyName	varchar(50)		not null,
    Remark		varchar(200)	null
);

create table Mapping
(
	Num			char(3)			not null primary key,
    CompanyId	char(6)			not null
);



CREATE VIEW `Agency_view`
as
	select Route.AgencyId, Agency.CompanyName, Route.Num as FlightId, Route.TotalNum,
           Flight.SetOff, Flight.Arrive, Area.AreaName as SouName, Area_1.AreaName as DesName, 
		   Route.Price, Flight.Cancel, Agency.Remark
	FROM   Area INNER JOIN
		   Agency INNER JOIN
           Route ON Agency.AgencyId = Route.AgencyId INNER JOIN
           Flight ON Route.Num = Flight.Num and Area.AreaId = Flight.SouId INNER JOIN 
           Area AS Area_1 ON Flight.DesId = Area_1.AreaId;

create view `Company_view`
as
	SELECT Mapping.CompanyId, Company.CompanyName, Mapping.Num, Company.Remark
	FROM   Mapping INNER JOIN
           Company ON Mapping.CompanyId = Company.CompanyId;

create view `Flight_view`
as
	SELECT Flight.Num, Area_1.AreaName, Area.AreaName AS Expr1, Flight.SetOff, Flight.Arrive, 
           Flight.Business - RealNum.Business AS Business, Flight.BusPrice * Flight.Discount AS BusPrice, 
           Flight.TotNum - RealNum.Normal - Flight.Business AS Normal, 
		   Flight.Price * Flight.Discount AS Price, Flight.Cancel
	FROM   Area INNER JOIN
           RealNum INNER JOIN
           Flight ON RealNum.Num = Flight.Num INNER JOIN
           Area AS Area_1 ON Flight.SouId = Area_1.AreaId and Area.AreaId = Flight.DesId;

create view `Info_view`
as
	SELECT  Info.Name, Info.Sex, Passinfo.Nickname, Passinfo.Password, Passinfo.Email, Info.Proof,
			Passinfo.Remark, Info.Oranization
	FROM    Info INNER JOIN
            Passinfo ON Info.Id = Passinfo.Id;


insert into Area values("000", "Nanjing");
insert into Area values("001", "Shanghai");
insert into Area values("002", "Beijing");
insert into Area values("003", "Tokoyo");
insert into Area values("004", "Los Angeles");
insert into Area values("005", "Florida");
insert into Area values("006", "Hokkaido");
insert into Area values("007", "Yokohama");
insert into Area values("008", "Venezia");
insert into Area values("009", "Rome");
insert into Area values("010", "London");
insert into Area values("100", "Sydney");

INSERT INTO `BookTickets`.`Flight` VALUES ('001', '001', '100', '2015-11-29 05:00:00', '2015-11-30 20:00:00', '400', '100', '6000', '8000', '1', 0);

insert into `BookTickets`.`Agency` values('000', 'Nanjing Company', '');

insert into `BookTickets`.`RealNum` values('001', '80', '250');

insert into `BookTickets`.`Route` values('000', '001', '100', '4000');
