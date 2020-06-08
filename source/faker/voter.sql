PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE voter (
	-- Unique identifier in the database.
	id INTEGER PRIMARY KEY,
	-- Last name
	lastname TEXT NOT NULL,
	-- Given names
	givennames TEXT NOT NULL,
	-- Residence address
	resaddress TEXT NOT NULL,
	-- Residence address line 2
	resaddress2 TEXT NOT NULL,
	-- Residence address zipcode
	reszip TEXT NOT NULL,
	-- Residence address state
	resstate TEXT NOT NULL,
	-- Mailing address
	mailaddress TEXT NOT NULL,
	-- Mailing address line 2
	mailaddress2 TEXT NOT NULL,
	-- Mailing zipcode
	mailzip TEXT NOT NULL,
	-- Mailing state
	mailstate TEXT NOT NULL,
	-- Registered party
	registeredparty TEXT NOT NULL,
	-- Birth date
	-- (Stored as a UNIX epoch value.)
	birthdate INTEGER NOT NULL,
	-- Identification information.
	idinfo BLOB UNIQUE NOT NULL,
	-- Registration status.
	status INTEGER NOT NULL,
	-- Initial registration time.
	-- (Stored as a UNIX epoch value.)
	initialregtime INTEGER NOT NULL,
	-- Last record update time.
	-- (Stored as a UNIX epoch value.)
	lastupdatetime INTEGER NOT NULL,
	-- Flag for voter confidentiality.
	confidential INTEGER NOT NULL
);
INSERT INTO voter VALUES(1,'Osborn','Steven','asdf','','9238','st','m addr1','','98123','sdf','SSITH',1591228800,X'616473666173646633343334',2,1591130704,1591130704,0);
INSERT INTO voter VALUES(2,'Osborn','Steven','1991 SE Brookwood','','97123','OR','1991 SE Brookwood','','97123','OR','SSITH',1592265600,X'61647366',2,1591130908,1591130908,0);
INSERT INTO voter VALUES(3,'Osborn','Steven','1991 SE Brookwood','','97123','OR','1991 SE Brookwood','','97123','OR','SSITH',1591660800,X'616466',2,1591133763,1591133763,0);
INSERT INTO voter VALUES(4,'Benson','Jacob',replace('USNS Clark\nFPO AA 80409','\n',char(10)),'','71647','NY',replace('USNS Clark\nFPO AA 80409','\n',char(10)),'','71647','NY','SSITH',953164800,'/path/to/4',0,0,0,0);
INSERT INTO voter VALUES(5,'Johnson','Jeffrey',replace('04024 Ricky Meadow\nWest Jordan, AK 92094','\n',char(10)),'','04587','ND',replace('04024 Ricky Meadow\nWest Jordan, AK 92094','\n',char(10)),'','04587','ND','SSITH',-1797033600,'/path/to/5',0,0,0,0);
INSERT INTO voter VALUES(6,'Todd','Dustin',replace('1448 David Shore\nLake Amberfurt, NY 05879','\n',char(10)),'','57071','KY',replace('1448 David Shore\nLake Amberfurt, NY 05879','\n',char(10)),'','57071','KY','SSITH',-227318400,'/path/to/6',1,0,0,0);
INSERT INTO voter VALUES(7,'Orozco','Christopher',replace('08970 Deleon Village\nLake Emilyberg, CT 73573','\n',char(10)),'','80429','OH',replace('08970 Deleon Village\nLake Emilyberg, CT 73573','\n',char(10)),'','80429','OH','SSITH',-6998400,'/path/to/7',1,0,0,0);
INSERT INTO voter VALUES(8,'Levy','Gabrielle',replace('371 Kimberly Hills\nHammondland, NY 86865','\n',char(10)),'','79353','IL',replace('371 Kimberly Hills\nHammondland, NY 86865','\n',char(10)),'','79353','IL','REBEL',-285379200,'/path/to/8',1,0,0,1);
INSERT INTO voter VALUES(9,'Fuller','Elizabeth',replace('140 Jacob Isle\nLake Brandon, AR 08209','\n',char(10)),'','42351','NH',replace('140 Jacob Isle\nLake Brandon, AR 08209','\n',char(10)),'','42351','NH','SSITH',467510400,'/path/to/9',1,0,0,1);
INSERT INTO voter VALUES(10,'Turner','Jason',replace('Unit 7668 Box 5863\nDPO AE 08396','\n',char(10)),'','82073','UT',replace('Unit 7668 Box 5863\nDPO AE 08396','\n',char(10)),'','82073','UT','REBEL',942364800,'/path/to/10',0,0,0,0);
INSERT INTO voter VALUES(11,'Jones','Michelle',replace('96753 Pennington Walks\nWilliamsland, OK 17125','\n',char(10)),'','03889','MN',replace('96753 Pennington Walks\nWilliamsland, OK 17125','\n',char(10)),'','03889','MN','REBEL',-1882742400,'/path/to/11',1,0,0,1);
INSERT INTO voter VALUES(12,'Anderson','Tricia',replace('3763 Ellis Key\nDianeville, CT 46303','\n',char(10)),'','78205','WV',replace('3763 Ellis Key\nDianeville, CT 46303','\n',char(10)),'','78205','WV','REBEL',-1348531200,'/path/to/12',1,0,0,0);
INSERT INTO voter VALUES(13,'Martin','Darrell',replace('8516 Mckay Islands Apt. 604\nMclaughlinland, NM 85250','\n',char(10)),'','57718','DE',replace('8516 Mckay Islands Apt. 604\nMclaughlinland, NM 85250','\n',char(10)),'','57718','DE','SSITH',-1319241600,'/path/to/13',0,0,0,0);
INSERT INTO voter VALUES(14,'Henderson','Crystal',replace('PSC 7939, Box 9219\nAPO AE 10477','\n',char(10)),'','33685','NM',replace('PSC 7939, Box 9219\nAPO AE 10477','\n',char(10)),'','33685','NM','SSITH',-1815091200,'/path/to/14',0,0,0,0);
INSERT INTO voter VALUES(15,'Carson','Lynn',replace('5733 Nathan Shore\nPort Adamville, TN 47303','\n',char(10)),'','42695','NC',replace('5733 Nathan Shore\nPort Adamville, TN 47303','\n',char(10)),'','42695','NC','REBEL',374284800,'/path/to/15',0,0,0,1);
INSERT INTO voter VALUES(16,'Knight','William',replace('462 Hernandez Mission\nSouth Joseph, IL 57416','\n',char(10)),'','01928','MD',replace('462 Hernandez Mission\nSouth Joseph, IL 57416','\n',char(10)),'','01928','MD','REBEL',723600000,'/path/to/16',0,0,0,1);
INSERT INTO voter VALUES(17,'Goodman','Keith',replace('44253 Jeffrey Oval\nNorth Timothy, SD 41042','\n',char(10)),'','30177','AK',replace('44253 Jeffrey Oval\nNorth Timothy, SD 41042','\n',char(10)),'','30177','AK','SSITH',-1070755200,'/path/to/17',1,0,0,1);
INSERT INTO voter VALUES(18,'Alvarado','Brittany',replace('100 Gomez Well\nNew Andre, VT 06474','\n',char(10)),'','51627','WA',replace('100 Gomez Well\nNew Andre, VT 06474','\n',char(10)),'','51627','WA','REBEL',874454400,'/path/to/18',1,0,0,1);
INSERT INTO voter VALUES(19,'Miles','Shawn',replace('352 Spencer Square\nNew Phillipland, DE 38571','\n',char(10)),'','62629','WI',replace('352 Spencer Square\nNew Phillipland, DE 38571','\n',char(10)),'','62629','WI','SSITH',-249436800,'/path/to/19',0,0,0,1);
INSERT INTO voter VALUES(20,'Moran','Dylan',replace('65501 Sierra Glens\nHannahshire, TX 13054','\n',char(10)),'','41037','OH',replace('65501 Sierra Glens\nHannahshire, TX 13054','\n',char(10)),'','41037','OH','SSITH',530323200,'/path/to/20',0,0,0,1);
INSERT INTO voter VALUES(21,'Baker','Thomas',replace('01670 Bennett Fort Apt. 683\nCampbellstad, RI 22986','\n',char(10)),'','67436','NM',replace('01670 Bennett Fort Apt. 683\nCampbellstad, RI 22986','\n',char(10)),'','67436','NM','REBEL',946857600,'/path/to/21',1,0,0,0);
INSERT INTO voter VALUES(22,'Roth','Gregory',replace('10605 Perry Drive\nDavestad, KY 50484','\n',char(10)),'','63538','NM',replace('10605 Perry Drive\nDavestad, KY 50484','\n',char(10)),'','63538','NM','REBEL',-723772800,'/path/to/22',1,0,0,1);
INSERT INTO voter VALUES(23,'Jones','Patricia',replace('4540 Jackson View Apt. 752\nPort Joannamouth, HI 58253','\n',char(10)),'','84794','IA',replace('4540 Jackson View Apt. 752\nPort Joannamouth, HI 58253','\n',char(10)),'','84794','IA','SSITH',974678400,'/path/to/23',1,0,0,1);
INSERT INTO voter VALUES(24,'Gomez','Justin',replace('9908 Scott Canyon Apt. 458\nEast Danielle, FL 59988','\n',char(10)),'','64579','NM',replace('9908 Scott Canyon Apt. 458\nEast Danielle, FL 59988','\n',char(10)),'','64579','NM','SSITH',575596800,'/path/to/24',1,0,0,1);
INSERT INTO voter VALUES(25,'Hodges','Gabriel',replace('4329 Ward Rest Suite 096\nGonzalezbury, SD 64548','\n',char(10)),'','11702','IN',replace('4329 Ward Rest Suite 096\nGonzalezbury, SD 64548','\n',char(10)),'','11702','IN','REBEL',799459200,'/path/to/25',0,0,0,0);
INSERT INTO voter VALUES(26,'Hinton','Nancy',replace('46432 Lauren Plain\nRichardsonstad, MA 47782','\n',char(10)),'','90631','KY',replace('46432 Lauren Plain\nRichardsonstad, MA 47782','\n',char(10)),'','90631','KY','SSITH',339120000,'/path/to/26',0,0,0,0);
INSERT INTO voter VALUES(27,'Coleman','Jacob',replace('47802 Joseph Ridge Suite 300\nSamanthaside, WI 34528','\n',char(10)),'','03677','GA',replace('47802 Joseph Ridge Suite 300\nSamanthaside, WI 34528','\n',char(10)),'','03677','GA','REBEL',-1894579200,'/path/to/27',0,0,0,0);
INSERT INTO voter VALUES(28,'Kaiser','Rebecca',replace('USNS Allen\nFPO AA 35358','\n',char(10)),'','20470','DC',replace('USNS Allen\nFPO AA 35358','\n',char(10)),'','20470','DC','SSITH',80092800,'/path/to/28',0,0,0,0);
INSERT INTO voter VALUES(29,'Baker','Brittney',replace('57724 Justin Squares\nNew Lori, NC 39366','\n',char(10)),'','02513','PA',replace('57724 Justin Squares\nNew Lori, NC 39366','\n',char(10)),'','02513','PA','REBEL',-1297123200,'/path/to/29',1,0,0,1);
INSERT INTO voter VALUES(30,'Rosario','Theresa',replace('16342 Krueger Coves Apt. 636\nSouth Georgestad, MS 71063','\n',char(10)),'','48655','ME',replace('16342 Krueger Coves Apt. 636\nSouth Georgestad, MS 71063','\n',char(10)),'','48655','ME','REBEL',-321148800,'/path/to/30',1,0,0,0);
INSERT INTO voter VALUES(31,'Webb','Janet',replace('110 Jade Divide Apt. 751\nJonathanshire, IL 17543','\n',char(10)),'','86550','AK',replace('110 Jade Divide Apt. 751\nJonathanshire, IL 17543','\n',char(10)),'','86550','AK','REBEL',482457600,'/path/to/31',0,0,0,1);
INSERT INTO voter VALUES(32,'Case','Cynthia',replace('2385 Payne Brook\nAllenport, NC 82899','\n',char(10)),'','87920','CA',replace('2385 Payne Brook\nAllenport, NC 82899','\n',char(10)),'','87920','CA','SSITH',373075200,'/path/to/32',1,0,0,0);
INSERT INTO voter VALUES(33,'Robinson','Ronald',replace('PSC 1994, Box 6565\nAPO AE 81552','\n',char(10)),'','91117','IN',replace('PSC 1994, Box 6565\nAPO AE 81552','\n',char(10)),'','91117','IN','SSITH',-830995200,'/path/to/33',1,0,0,1);
INSERT INTO voter VALUES(34,'Vazquez','Sheila',replace('6087 Rebecca Lock Apt. 494\nKarinachester, NY 85175','\n',char(10)),'','56329','KS',replace('6087 Rebecca Lock Apt. 494\nKarinachester, NY 85175','\n',char(10)),'','56329','KS','SSITH',117331200,'/path/to/34',1,0,0,1);
INSERT INTO voter VALUES(35,'Berry','Ronald',replace('354 Joshua Estates Suite 425\nLake Donald, GA 96496','\n',char(10)),'','76258','NE',replace('354 Joshua Estates Suite 425\nLake Donald, GA 96496','\n',char(10)),'','76258','NE','REBEL',-1001116800,'/path/to/35',0,0,0,0);
INSERT INTO voter VALUES(36,'Le','Kimberly',replace('5266 Lisa Pine Apt. 155\nJohnstonborough, OR 97666','\n',char(10)),'','43168','FL',replace('5266 Lisa Pine Apt. 155\nJohnstonborough, OR 97666','\n',char(10)),'','43168','FL','SSITH',-1719100800,'/path/to/36',0,0,0,1);
INSERT INTO voter VALUES(37,'Watson','Jimmy',replace('92219 Jamie Light Apt. 900\nSouth Jenniferfort, CT 15051','\n',char(10)),'','37383','KS',replace('92219 Jamie Light Apt. 900\nSouth Jenniferfort, CT 15051','\n',char(10)),'','37383','KS','SSITH',-1229040000,'/path/to/37',1,0,0,0);
INSERT INTO voter VALUES(38,'Taylor','Steven',replace('Unit 5312 Box 2989\nDPO AP 23994','\n',char(10)),'','55219','IN',replace('Unit 5312 Box 2989\nDPO AP 23994','\n',char(10)),'','55219','IN','REBEL',-113097600,'/path/to/38',1,0,0,0);
INSERT INTO voter VALUES(39,'Le','Alexandra',replace('PSC 7364, Box 6690\nAPO AP 87734','\n',char(10)),'','09407','CT',replace('PSC 7364, Box 6690\nAPO AP 87734','\n',char(10)),'','09407','CT','REBEL',603158400,'/path/to/39',0,0,0,1);
INSERT INTO voter VALUES(40,'Gross','Jennifer',replace('4422 Jacob Stravenue\nEast Valerie, OH 68693','\n',char(10)),'','72253','AR',replace('4422 Jacob Stravenue\nEast Valerie, OH 68693','\n',char(10)),'','72253','AR','REBEL',-1229472000,'/path/to/40',1,0,0,1);
INSERT INTO voter VALUES(41,'Crawford','Michael',replace('696 Butler Land Apt. 837\nGriffithberg, ID 81905','\n',char(10)),'','57666','WI',replace('696 Butler Land Apt. 837\nGriffithberg, ID 81905','\n',char(10)),'','57666','WI','SSITH',472780800,'/path/to/41',0,0,0,0);
INSERT INTO voter VALUES(42,'Morris','Paul',replace('674 Mitchell Rest Suite 685\nNorth Danielberg, PA 42645','\n',char(10)),'','15294','IL',replace('674 Mitchell Rest Suite 685\nNorth Danielberg, PA 42645','\n',char(10)),'','15294','IL','SSITH',-1222300800,'/path/to/42',1,0,0,1);
INSERT INTO voter VALUES(43,'Gates','Jamie',replace('8095 Gonzalez Turnpike Suite 858\nEast Patricia, KY 17692','\n',char(10)),'','71613','NC',replace('8095 Gonzalez Turnpike Suite 858\nEast Patricia, KY 17692','\n',char(10)),'','71613','NC','REBEL',613958400,'/path/to/43',0,0,0,0);
INSERT INTO voter VALUES(44,'Miller','Brian',replace('Unit 8870 Box 7045\nDPO AE 71687','\n',char(10)),'','73684','OK',replace('Unit 8870 Box 7045\nDPO AE 71687','\n',char(10)),'','73684','OK','SSITH',-238809600,'/path/to/44',1,0,0,0);
INSERT INTO voter VALUES(45,'May','Anthony',replace('025 Bernard Spur\nNew Donaldstad, MI 36220','\n',char(10)),'','24627','OR',replace('025 Bernard Spur\nNew Donaldstad, MI 36220','\n',char(10)),'','24627','OR','SSITH',-1074297600,'/path/to/45',1,0,0,1);
INSERT INTO voter VALUES(46,'Morgan','Cheryl',replace('12582 Holmes River\nWest Robertmouth, DE 72336','\n',char(10)),'','18238','IL',replace('12582 Holmes River\nWest Robertmouth, DE 72336','\n',char(10)),'','18238','IL','REBEL',-1178496000,'/path/to/46',1,0,0,0);
INSERT INTO voter VALUES(47,'Norton','Emma',replace('PSC 2048, Box 7334\nAPO AE 42620','\n',char(10)),'','50561','SC',replace('PSC 2048, Box 7334\nAPO AE 42620','\n',char(10)),'','50561','SC','REBEL',-888105600,'/path/to/47',1,0,0,0);
INSERT INTO voter VALUES(48,'Cooke','Joseph',replace('31747 Moore Heights Apt. 815\nDelgadoton, MD 86446','\n',char(10)),'','93324','DC',replace('31747 Moore Heights Apt. 815\nDelgadoton, MD 86446','\n',char(10)),'','93324','DC','REBEL',900460800,'/path/to/48',1,0,0,1);
INSERT INTO voter VALUES(49,'Reed','Joshua',replace('142 Reyes Parkways\nSouth Timothyfurt, TN 27226','\n',char(10)),'','33684','DE',replace('142 Reyes Parkways\nSouth Timothyfurt, TN 27226','\n',char(10)),'','33684','DE','SSITH',640828800,'/path/to/49',0,0,0,1);
INSERT INTO voter VALUES(50,'Jones','Laura',replace('99890 William Heights\nLake Colleen, NY 53065','\n',char(10)),'','71323','MT',replace('99890 William Heights\nLake Colleen, NY 53065','\n',char(10)),'','71323','MT','REBEL',-1861315200,'/path/to/50',0,0,0,0);
INSERT INTO voter VALUES(51,'Green','Justin',replace('0650 Adams Turnpike Suite 182\nRodneyborough, SD 63944','\n',char(10)),'','64847','NH',replace('0650 Adams Turnpike Suite 182\nRodneyborough, SD 63944','\n',char(10)),'','64847','NH','SSITH',1009065600,'/path/to/51',0,0,0,0);
INSERT INTO voter VALUES(52,'Dyer','Christopher',replace('9911 Zachary Pines Suite 681\nWilsonfurt, MS 68003','\n',char(10)),'','43588','OK',replace('9911 Zachary Pines Suite 681\nWilsonfurt, MS 68003','\n',char(10)),'','43588','OK','SSITH',-1493942400,'/path/to/52',1,0,0,1);
INSERT INTO voter VALUES(53,'Lane','Jeffrey',replace('632 Ortiz Stream\nNorth Jesus, HI 26837','\n',char(10)),'','09433','WV',replace('632 Ortiz Stream\nNorth Jesus, HI 26837','\n',char(10)),'','09433','WV','REBEL',635990400,'/path/to/53',0,0,0,0);
INSERT INTO voter VALUES(54,'Hall','Michael',replace('2712 Coleman Crest\nWest Christopher, NC 81933','\n',char(10)),'','40246','NV',replace('2712 Coleman Crest\nWest Christopher, NC 81933','\n',char(10)),'','40246','NV','REBEL',645321600,'/path/to/54',0,0,0,0);
INSERT INTO voter VALUES(55,'Carr','Melissa',replace('Unit 4187 Box 3874\nDPO AA 63568','\n',char(10)),'','78384','MO',replace('Unit 4187 Box 3874\nDPO AA 63568','\n',char(10)),'','78384','MO','REBEL',308275200,'/path/to/55',0,0,0,0);
INSERT INTO voter VALUES(56,'Lam','Audrey',replace('8459 Dunn Estates\nNorth Christopherburgh, UT 13784','\n',char(10)),'','21968','AZ',replace('8459 Dunn Estates\nNorth Christopherburgh, UT 13784','\n',char(10)),'','21968','AZ','REBEL',-1178150400,'/path/to/56',0,0,0,0);
INSERT INTO voter VALUES(57,'Sparks','Joseph',replace('588 Jackson Course Suite 518\nSouth Shelley, WI 11260','\n',char(10)),'','83662','FL',replace('588 Jackson Course Suite 518\nSouth Shelley, WI 11260','\n',char(10)),'','83662','FL','REBEL',-598406400,'/path/to/57',1,0,0,0);
INSERT INTO voter VALUES(58,'Gregory','Nicole',replace('83147 Natalie Junction\nNew Rhondamouth, KY 37834','\n',char(10)),'','41504','OH',replace('83147 Natalie Junction\nNew Rhondamouth, KY 37834','\n',char(10)),'','41504','OH','SSITH',-354067200,'/path/to/58',0,0,0,1);
INSERT INTO voter VALUES(59,'Bradford','Courtney',replace('63612 Cherry Crossroad\nLake Tyler, WY 57613','\n',char(10)),'','08996','CO',replace('63612 Cherry Crossroad\nLake Tyler, WY 57613','\n',char(10)),'','08996','CO','REBEL',-555897600,'/path/to/59',0,0,0,0);
INSERT INTO voter VALUES(60,'Fowler','Larry',replace('32584 Williams Knoll Apt. 106\nWest Theresa, AK 40781','\n',char(10)),'','13521','LA',replace('32584 Williams Knoll Apt. 106\nWest Theresa, AK 40781','\n',char(10)),'','13521','LA','REBEL',-933033600,'/path/to/60',1,0,0,0);
INSERT INTO voter VALUES(61,'Wells','Joel',replace('23910 Davis Islands Suite 548\nLopezfurt, MI 22655','\n',char(10)),'','31314','ME',replace('23910 Davis Islands Suite 548\nLopezfurt, MI 22655','\n',char(10)),'','31314','ME','REBEL',-1439856000,'/path/to/61',1,0,0,0);
INSERT INTO voter VALUES(62,'Kaufman','Chad',replace('713 Steven Rapid\nAndrewview, KS 82624','\n',char(10)),'','01646','SC',replace('713 Steven Rapid\nAndrewview, KS 82624','\n',char(10)),'','01646','SC','REBEL',509673600,'/path/to/62',1,0,0,0);
INSERT INTO voter VALUES(63,'Obrien','Claudia',replace('Unit 9508 Box 9210\nDPO AA 59330','\n',char(10)),'','37552','MO',replace('Unit 9508 Box 9210\nDPO AA 59330','\n',char(10)),'','37552','MO','REBEL',-1824249600,'/path/to/63',0,0,0,1);
INSERT INTO voter VALUES(64,'Cantrell','Larry',replace('19719 Paul Tunnel Suite 059\nStokesborough, IL 38157','\n',char(10)),'','96806','CT',replace('19719 Paul Tunnel Suite 059\nStokesborough, IL 38157','\n',char(10)),'','96806','CT','SSITH',-1756944000,'/path/to/64',0,0,0,1);
INSERT INTO voter VALUES(65,'Hill','Julie',replace('41583 Harmon Rue\nNew Randy, TX 10605','\n',char(10)),'','68154','AK',replace('41583 Harmon Rue\nNew Randy, TX 10605','\n',char(10)),'','68154','AK','REBEL',132278400,'/path/to/65',1,0,0,0);
INSERT INTO voter VALUES(66,'Wood','Virginia',replace('46329 Turner Ferry\nLake Lisaton, TX 59309','\n',char(10)),'','65219','CA',replace('46329 Turner Ferry\nLake Lisaton, TX 59309','\n',char(10)),'','65219','CA','REBEL',-1402617600,'/path/to/66',1,0,0,1);
INSERT INTO voter VALUES(67,'Obrien','Patrick',replace('36687 Brandon Neck Apt. 137\nLake Keith, TN 97240','\n',char(10)),'','47650','WA',replace('36687 Brandon Neck Apt. 137\nLake Keith, TN 97240','\n',char(10)),'','47650','WA','REBEL',-638928000,'/path/to/67',0,0,0,1);
INSERT INTO voter VALUES(68,'Mitchell','Sherry',replace('7039 Morgan Wall Suite 031\nNew Amyview, SC 05134','\n',char(10)),'','01872','MT',replace('7039 Morgan Wall Suite 031\nNew Amyview, SC 05134','\n',char(10)),'','01872','MT','SSITH',-1189036800,'/path/to/68',1,0,0,1);
INSERT INTO voter VALUES(69,'Braun','Anna',replace('357 Brown Fort Apt. 957\nEast Alisonmouth, GA 46563','\n',char(10)),'','69004','NC',replace('357 Brown Fort Apt. 957\nEast Alisonmouth, GA 46563','\n',char(10)),'','69004','NC','SSITH',-1785715200,'/path/to/69',1,0,0,1);
INSERT INTO voter VALUES(70,'Jones','Joshua',replace('Unit 4096 Box 7856\nDPO AP 47812','\n',char(10)),'','67621','OH',replace('Unit 4096 Box 7856\nDPO AP 47812','\n',char(10)),'','67621','OH','SSITH',66700800,'/path/to/70',1,0,0,0);
INSERT INTO voter VALUES(71,'Foster','Mary',replace('Unit 1993 Box 2943\nDPO AE 85692','\n',char(10)),'','77252','ID',replace('Unit 1993 Box 2943\nDPO AE 85692','\n',char(10)),'','77252','ID','REBEL',-1239926400,'/path/to/71',0,0,0,1);
INSERT INTO voter VALUES(72,'Bradford','Sean',replace('5715 Sutton Tunnel Apt. 219\nTimothyton, ND 14222','\n',char(10)),'','50807','IA',replace('5715 Sutton Tunnel Apt. 219\nTimothyton, ND 14222','\n',char(10)),'','50807','IA','REBEL',699753600,'/path/to/72',1,0,0,0);
INSERT INTO voter VALUES(73,'Stanley','Kimberly',replace('415 Elizabeth Spur Apt. 656\nCunninghamborough, NJ 36538','\n',char(10)),'','38286','SC',replace('415 Elizabeth Spur Apt. 656\nCunninghamborough, NJ 36538','\n',char(10)),'','38286','SC','REBEL',-959040000,'/path/to/73',1,0,0,1);
INSERT INTO voter VALUES(74,'Burns','Desiree',replace('382 Rose Run Suite 033\nScottmouth, NC 86978','\n',char(10)),'','73595','PA',replace('382 Rose Run Suite 033\nScottmouth, NC 86978','\n',char(10)),'','73595','PA','SSITH',408931200,'/path/to/74',0,0,0,1);
INSERT INTO voter VALUES(75,'Moody','Heidi',replace('83306 Nicholas Courts Suite 128\nCarlosborough, AL 52459','\n',char(10)),'','42248','VA',replace('83306 Nicholas Courts Suite 128\nCarlosborough, AL 52459','\n',char(10)),'','42248','VA','REBEL',-1378512000,'/path/to/75',1,0,0,0);
INSERT INTO voter VALUES(76,'Welch','Angela',replace('38354 Nancy Expressway Apt. 966\nEast Ashleyburgh, LA 86681','\n',char(10)),'','13202','SC',replace('38354 Nancy Expressway Apt. 966\nEast Ashleyburgh, LA 86681','\n',char(10)),'','13202','SC','SSITH',154828800,'/path/to/76',1,0,0,0);
INSERT INTO voter VALUES(77,'Neal','John',replace('60613 Glenn Lane Suite 356\nNorth Maryville, KY 16901','\n',char(10)),'','26273','IL',replace('60613 Glenn Lane Suite 356\nNorth Maryville, KY 16901','\n',char(10)),'','26273','IL','REBEL',-283737600,'/path/to/77',0,0,0,1);
INSERT INTO voter VALUES(78,'Chaney','Michael',replace('98994 Stevens Roads\nAndersonshire, TX 94630','\n',char(10)),'','77467','MO',replace('98994 Stevens Roads\nAndersonshire, TX 94630','\n',char(10)),'','77467','MO','SSITH',-1463184000,'/path/to/78',0,0,0,1);
INSERT INTO voter VALUES(79,'Allen','Donald',replace('3047 Miles Park Suite 251\nPort Chrisburgh, AR 17572','\n',char(10)),'','88076','NJ',replace('3047 Miles Park Suite 251\nPort Chrisburgh, AR 17572','\n',char(10)),'','88076','NJ','SSITH',-680659200,'/path/to/79',0,0,0,1);
INSERT INTO voter VALUES(80,'Smith','Ryan',replace('941 Jerome Gardens Suite 004\nJohnstad, VT 11162','\n',char(10)),'','22620','SD',replace('941 Jerome Gardens Suite 004\nJohnstad, VT 11162','\n',char(10)),'','22620','SD','SSITH',-1778544000,'/path/to/80',0,0,0,1);
INSERT INTO voter VALUES(81,'Hawkins','Victoria',replace('49897 Roy Gateway\nNew Jaclyn, SD 61108','\n',char(10)),'','79577','ID',replace('49897 Roy Gateway\nNew Jaclyn, SD 61108','\n',char(10)),'','79577','ID','REBEL',-1812931200,'/path/to/81',1,0,0,1);
INSERT INTO voter VALUES(82,'Moreno','Yolanda',replace('3732 Jeffrey Turnpike Suite 794\nPort Mary, ID 26037','\n',char(10)),'','02052','NM',replace('3732 Jeffrey Turnpike Suite 794\nPort Mary, ID 26037','\n',char(10)),'','02052','NM','REBEL',-573782400,'/path/to/82',0,0,0,1);
INSERT INTO voter VALUES(83,'Mathews','Robert',replace('2685 Riley Canyon Apt. 289\nCunninghamport, OR 36247','\n',char(10)),'','84501','UT',replace('2685 Riley Canyon Apt. 289\nCunninghamport, OR 36247','\n',char(10)),'','84501','UT','SSITH',520992000,'/path/to/83',0,0,0,0);
INSERT INTO voter VALUES(84,'Hart','Daniel',replace('1957 Phillips Street Suite 469\nWardberg, WA 37852','\n',char(10)),'','07619','CO',replace('1957 Phillips Street Suite 469\nWardberg, WA 37852','\n',char(10)),'','07619','CO','REBEL',-1259539200,'/path/to/84',1,0,0,1);
INSERT INTO voter VALUES(85,'Carr','Karen',replace('119 York Knoll Apt. 750\nGutierrezville, IA 83696','\n',char(10)),'','18488','RI',replace('119 York Knoll Apt. 750\nGutierrezville, IA 83696','\n',char(10)),'','18488','RI','SSITH',-939081600,'/path/to/85',1,0,0,1);
INSERT INTO voter VALUES(86,'Cox','Margaret',replace('36502 Nicholas Bridge\nAmberton, OH 12857','\n',char(10)),'','61799','OR',replace('36502 Nicholas Bridge\nAmberton, OH 12857','\n',char(10)),'','61799','OR','SSITH',885686400,'/path/to/86',1,0,0,1);
INSERT INTO voter VALUES(87,'Mcguire','Joseph',replace('75879 Nelson Extensions\nNew Katieton, FL 06588','\n',char(10)),'','28701','TN',replace('75879 Nelson Extensions\nNew Katieton, FL 06588','\n',char(10)),'','28701','TN','REBEL',-1408060800,'/path/to/87',0,0,0,1);
INSERT INTO voter VALUES(88,'Duncan','Michele',replace('1293 Scott Fords\nSuzanneberg, CT 97343','\n',char(10)),'','22564','TN',replace('1293 Scott Fords\nSuzanneberg, CT 97343','\n',char(10)),'','22564','TN','SSITH',755395200,'/path/to/88',1,0,0,0);
INSERT INTO voter VALUES(89,'Waller','Christopher',replace('753 Hawkins Roads Suite 832\nNew Janice, NH 49908','\n',char(10)),'','78513','MA',replace('753 Hawkins Roads Suite 832\nNew Janice, NH 49908','\n',char(10)),'','78513','MA','SSITH',-1650067200,'/path/to/89',1,0,0,1);
INSERT INTO voter VALUES(90,'Sanchez','Emma',replace('474 Dominguez Row\nStevensonburgh, AR 05093','\n',char(10)),'','86684','MN',replace('474 Dominguez Row\nStevensonburgh, AR 05093','\n',char(10)),'','86684','MN','REBEL',-1718582400,'/path/to/90',0,0,0,0);
INSERT INTO voter VALUES(91,'Smith','Tara',replace('191 Knight Ridge Apt. 163\nJamesport, FL 29997','\n',char(10)),'','38193','LA',replace('191 Knight Ridge Apt. 163\nJamesport, FL 29997','\n',char(10)),'','38193','LA','REBEL',-1829865600,'/path/to/91',0,0,0,0);
INSERT INTO voter VALUES(92,'Taylor','Lisa',replace('1088 Murphy Passage Suite 332\nLaurenville, NV 14257','\n',char(10)),'','92414','MO',replace('1088 Murphy Passage Suite 332\nLaurenville, NV 14257','\n',char(10)),'','92414','MO','SSITH',-1423612800,'/path/to/92',1,0,0,1);
INSERT INTO voter VALUES(93,'Alvarez','Paul',replace('113 Moore Lakes Suite 300\nWest Henry, MD 55116','\n',char(10)),'','83172','WV',replace('113 Moore Lakes Suite 300\nWest Henry, MD 55116','\n',char(10)),'','83172','WV','SSITH',-1812412800,'/path/to/93',0,0,0,0);
INSERT INTO voter VALUES(94,'Miller','Brandon',replace('8111 Alvarez Estate Apt. 067\nLake Rogerland, GA 60621','\n',char(10)),'','86849','CA',replace('8111 Alvarez Estate Apt. 067\nLake Rogerland, GA 60621','\n',char(10)),'','86849','CA','REBEL',14688000,'/path/to/94',1,0,0,0);
INSERT INTO voter VALUES(95,'Foster','Kimberly',replace('52171 Bradley Lodge Apt. 123\nSmithbury, NY 29023','\n',char(10)),'','15650','ME',replace('52171 Bradley Lodge Apt. 123\nSmithbury, NY 29023','\n',char(10)),'','15650','ME','SSITH',573436800,'/path/to/95',0,0,0,0);
INSERT INTO voter VALUES(96,'Johnson','Jeremy',replace('97257 Allen Orchard\nWest Kevin, LA 45259','\n',char(10)),'','09786','GA',replace('97257 Allen Orchard\nWest Kevin, LA 45259','\n',char(10)),'','09786','GA','SSITH',-2029708800,'/path/to/96',0,0,0,0);
INSERT INTO voter VALUES(97,'Boyer','Angela',replace('80706 Simon Lock Apt. 436\nNorth Danny, AK 46551','\n',char(10)),'','17954','VT',replace('80706 Simon Lock Apt. 436\nNorth Danny, AK 46551','\n',char(10)),'','17954','VT','REBEL',-307756800,'/path/to/97',0,0,0,0);
INSERT INTO voter VALUES(98,'Griffith','William',replace('936 Preston Motorway Apt. 692\nCochranmouth, MN 95706','\n',char(10)),'','10150','UT',replace('936 Preston Motorway Apt. 692\nCochranmouth, MN 95706','\n',char(10)),'','10150','UT','REBEL',-1354752000,'/path/to/98',0,0,0,1);
INSERT INTO voter VALUES(99,'Johnson','Kenneth',replace('9373 Nicole Square Suite 174\nNew Josephburgh, OR 72463','\n',char(10)),'','29519','TX',replace('9373 Nicole Square Suite 174\nNew Josephburgh, OR 72463','\n',char(10)),'','29519','TX','SSITH',-1580169600,'/path/to/99',0,0,0,1);
INSERT INTO voter VALUES(100,'Robinson','Heather',replace('13523 Sanders Ports\nLake Dakota, ND 63088','\n',char(10)),'','47725','CA',replace('13523 Sanders Ports\nLake Dakota, ND 63088','\n',char(10)),'','47725','CA','REBEL',-982886400,'/path/to/100',1,0,0,1);
INSERT INTO voter VALUES(101,'Alvarado','Kendra',replace('6655 Angelica Plaza Suite 984\nEast Heather, NC 92561','\n',char(10)),'','73903','CT',replace('6655 Angelica Plaza Suite 984\nEast Heather, NC 92561','\n',char(10)),'','73903','CT','SSITH',-25488000,'/path/to/101',1,0,0,1);
INSERT INTO voter VALUES(102,'Newman','Debra',replace('6566 Kimberly Harbors\nWilsonview, LA 68748','\n',char(10)),'','02158','WI',replace('6566 Kimberly Harbors\nWilsonview, LA 68748','\n',char(10)),'','02158','WI','SSITH',-1195344000,'/path/to/102',1,0,0,0);
INSERT INTO voter VALUES(103,'Jackson','Christopher',replace('USNV Williams\nFPO AP 50891','\n',char(10)),'','19642','VT',replace('USNV Williams\nFPO AP 50891','\n',char(10)),'','19642','VT','SSITH',-539568000,'/path/to/103',1,0,0,0);
INSERT INTO voter VALUES(104,'Davidson','Tammy',replace('575 Johnson Trail\nWest Joannashire, DC 71711','\n',char(10)),'','67997','AR',replace('575 Johnson Trail\nWest Joannashire, DC 71711','\n',char(10)),'','67997','AR','SSITH',-1439164800,'/path/to/104',1,0,0,1);
INSERT INTO voter VALUES(105,'Camacho','Alex',replace('31948 Harvey Summit Apt. 142\nCharlesbury, RI 99719','\n',char(10)),'','04333','OK',replace('31948 Harvey Summit Apt. 142\nCharlesbury, RI 99719','\n',char(10)),'','04333','OK','SSITH',-1074902400,'/path/to/105',0,0,0,1);
INSERT INTO voter VALUES(106,'Thomas','Rebecca',replace('PSC 2751, Box 8971\nAPO AP 25110','\n',char(10)),'','12452','AZ',replace('PSC 2751, Box 8971\nAPO AP 25110','\n',char(10)),'','12452','AZ','SSITH',-233193600,'/path/to/106',0,0,0,1);
INSERT INTO voter VALUES(107,'Wheeler','Tammy',replace('Unit 7496 Box 7731\nDPO AP 55140','\n',char(10)),'','57821','HI',replace('Unit 7496 Box 7731\nDPO AP 55140','\n',char(10)),'','57821','HI','REBEL',79401600,'/path/to/107',0,0,0,0);
INSERT INTO voter VALUES(108,'Anderson','Todd',replace('27892 Patrick View Suite 804\nRussellchester, GA 63153','\n',char(10)),'','19458','ID',replace('27892 Patrick View Suite 804\nRussellchester, GA 63153','\n',char(10)),'','19458','ID','SSITH',-26438400,'/path/to/108',1,0,0,1);
INSERT INTO voter VALUES(109,'Mayer','Edward',replace('94000 Samantha Views Apt. 687\nMelindaside, ME 15299','\n',char(10)),'','87531','AK',replace('94000 Samantha Views Apt. 687\nMelindaside, ME 15299','\n',char(10)),'','87531','AK','SSITH',679104000,'/path/to/109',1,0,0,1);
COMMIT;
