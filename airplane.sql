/*
 Navicat MySQL Data Transfer

 Source Server         : 172.16.172.6
 Source Server Type    : MySQL
 Source Server Version : 50528
 Source Host           : 172.16.172.6
 Source Database       : airplane

 Target Server Type    : MySQL
 Target Server Version : 50528
 File Encoding         : utf-8

 Date: 08/12/2015 15:43:21 PM
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `auth`
-- ----------------------------
DROP TABLE IF EXISTS `auth`;
CREATE TABLE `auth` (
  `UserID` varchar(20) NOT NULL,
  `AccToken` char(32) NOT NULL,
  PRIMARY KEY (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `userequip`
-- ----------------------------
DROP TABLE IF EXISTS `userequip`;
CREATE TABLE `userequip` (
  `equipid` int(32) unsigned NOT NULL AUTO_INCREMENT,
  `userid` varchar(32) NOT NULL,
  `status` tinyint(8) unsigned NOT NULL,
  `type` tinyint(8) unsigned NOT NULL,
  `modelname` varchar(16) NOT NULL,
  `level` tinyint(8) unsigned NOT NULL,
  `exp` mediumint(24) NOT NULL,
  PRIMARY KEY (`equipid`,`userid`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `userinfo`
-- ----------------------------
DROP TABLE IF EXISTS `userinfo`;
CREATE TABLE `userinfo` (
  `userid` varchar(32) NOT NULL,
  `userexp` int(20) unsigned NOT NULL,
  `money` int(20) unsigned NOT NULL,
  `dimond` smallint(12) unsigned NOT NULL,
  `power` smallint(8) unsigned NOT NULL,
  `vip` tinyint(4) unsigned NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Procedure structure for `proc_addoneE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_addoneE`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_addoneE`(IN uid VARCHAR(20),IN t TINYINT(8),IN m VARCHAR(16),IN l SMALLINT(16),IN e MEDIUMINT

(24))
BEGIN
  DECLARE eid INT;
  START TRANSACTION;
  INSERT INTO airplane.userequip(userid,status,type,modelname,level,exp) VALUES (uid,0,t,m,l,e);
  SELECT LAST_INSERT_ID() INTO eid;
  SELECT eid;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_changeE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_changeE`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_changeE`(IN eid int(32),IN uid varchar(20))
BEGIN
  DECLARE t TINYINT;
  DECLARE s TINYINT;
  DECLARE code TINYINT;
  START TRANSACTION;
  IF NOT EXISTS (SELECT * FROM airplane.userequip WHERE userequip.equipid = eid) THEN
   SET code = 0;
   SELECT code;
  ELSE
   SELECT status INTO s FROM airplane.userequip WHERE userequip.equipid = eid;
	IF s = 1 THEN
	  SET code = 0;
	  SELECT code;
        ELSE
          SELECT userequip.type INTO t FROM airplane.userequip WHERE userequip.equipid = eid; 
          IF t<10 THEN
             UPDATE airplane.userequip SET userequip.status = 0 WHERE userid = uid AND type = t AND status = 1;
             UPDATE airplane.userequip SET userequip.status = 1 WHERE equipid = eid;
             SELECT * FROM airplane.userequip WHERE equipid = eid;
          END IF;
        END IF;
  END IF;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_getEids`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_getEids`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_getEids`(IN uid VARCHAR(20),IN n INT(32),IN eids VARCHAR(128))
BEGIN
  DECLARE k INT;
  DECLARE code TINYINT;
  START TRANSACTION;
  SELECT COUNT(*) INTO k FROM airplane.userequip WHERE FIND_IN_SET(equipid,(eids)) AND userid = uid LIMIT n;
  IF k<>n THEN
    SET code = 0;
    SELECT code;
  ELSE
    SELECT * FROM airplane.userequip WHERE FIND_IN_SET(equipid,(eids)) AND userid = uid LIMIT n;
  END IF;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_getEinfo`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_getEinfo`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_getEinfo`(IN uid varchar(20))
BEGIN
  START TRANSACTION;
  SELECT * FROM airplane.userequip WHERE userequip.userid = uid;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_getEtype`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_getEtype`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `proc_getEtype`(IN eid int(32))
BEGIN
  START TRANSACTION;
  SELECT type FROM airplane.userequip WHERE userequip.EquipID = eid;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_getUinfo`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_getUinfo`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_getUinfo`(IN uid varchar(20))
BEGIN   
  START TRANSACTION;
  IF NOT EXISTS (SELECT * FROM airplane.userinfo WHERE userinfo.userid = uid) THEN
     INSERT INTO airplane.userinfo VALUES (uid,0,1888,88,80,0);
     INSERT INTO airplane.userequip (userid,status,type,modelname,level,exp) VALUES (uid,1,0,'zhanshenyihao',1,0),(uid,0,0,'xunleijianfeng',1,0),
(uid,1,1,'bazhuahudun',1,0),(uid,0,1,'bazhuahudun',1,0),(uid,1,2,'jiutoushedujian',1,0),(uid,0,2,'wujinchuanjiadan',1,0),(uid,1,3,'qingtianbazhua',1,0),(uid,0,3,'sidanjiguanpao',1,0);
  END IF;
  SELECT * FROM airplane.userinfo WHERE userid = uid;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_pvp_replace`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_pvp_replace`;
delimiter ;;
CREATE DEFINER=`root`@`%` PROCEDURE `proc_pvp_replace`(IN uid varchar(20), IN equip_id int(20))
BEGIN
  DECLARE t TINYINT;
  DECLARE s TINYINT;
  DECLARE code TINYINT;
  START TRANSACTION;
  IF NOT EXISTS (SELECT * FROM airplane.userequip WHERE userequip.equipid = eid) THEN
   SET code = 0;
   SELECT code;
  ELSE
   SELECT status INTO s FROM airplane.userequip WHERE userequip.equipid = eid;
	IF s = 1 THEN
	  SET code = 0;
	  SELECT code;
        ELSE
          SELECT userequip.type INTO t FROM airplane.userequip WHERE userequip.equipid = eid; 
          IF t<10 THEN
             UPDATE airplane.userequip SET userequip.status = 0 WHERE userid = uid AND type = t AND status = 1;
             UPDATE airplane.userequip SET userequip.status = 1 WHERE equipid = eid;
             SELECT * FROM airplane.userequip WHERE equipid = eid;
          END IF;
        END IF;
  END IF;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_sellE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_sellE`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_sellE`(IN uid VARCHAR(20),IN n int(32),IN eids VARCHAR(2048))
BEGIN
  DECLARE k INT(32);
  DECLARE l INT;
  DECLARE code TINYINT;
  DECLARE m INT;
  DECLARE m1 INT;
  DECLARE m2 INT;
  START TRANSACTION;
  SELECT COUNT(*) INTO k FROM airplane.userequip WHERE FIND_IN_SET (equipid,(eids)) AND userid = uid AND status != 1 LIMIT n;
  IF k!=n THEN
    SET code = 0;
    SELECT code;
  ELSE
    SELECT SUM(level) INTO l FROM airplane.userequip WHERE FIND_IN_SET (equipid,(eids)) LIMIT n;
    SET m1 = 40 * n + 2 * l;
    DELETE FROM airplane.userequip WHERE FIND_IN_SET (equipid,(eids)) LIMIT n;
    SELECT money INTO m2 FROM airplane.userinfo WHERE userinfo.userid = uid;
    SET m = m1 + m2;
    UPDATE airplane.userinfo SET money = m WHERE userid = uid;
    SELECT m;
  END IF;
  COMMIT;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_test`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_test`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_test`(IN n int(32),IN eids VARCHAR(64))
BEGIN
  DECLARE k INT;
  SELECT COUNT(*) AS k FROM airplane.userequip WHERE FIND_IN_SET (equipid,(eids)) LIMIT n;
END
 ;;
delimiter ;

-- ----------------------------
--  Procedure structure for `proc_upgradeE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `proc_upgradeE`;
delimiter ;;
CREATE DEFINER=`root`@`127.0.0.1` PROCEDURE `proc_upgradeE`(IN uid VARCHAR(20),IN eid INT(64),IN exp MEDIUMINT(24),IN l SMALLINT(16),IN eids 

VARCHAR(128),IN m MEDIUMINT(24))
BEGIN
  DECLARE m1 INT;
  DECLARE m2 INT;
  START TRANSACTION;
  SELECT money INTO m1 FROM airplane.userinfo WHERE userinfo.userid = uid;
  SET m2 = m1 - m;
  IF m2<0 THEN
    SET m = 0;
  ELSE 
    UPDATE airplane.userinfo SET userinfo.money = m2 WHERE userid = uid;
    UPDATE airplane.userequip SET userequip.level = l,userequip.exp = exp WHERE userequip.equipid = eid;
    DELETE FROM airplane.userequip WHERE FIND_IN_SET(equipid,(eids));
  END IF;
    SELECT m;
  COMMIT;
END
 ;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;
