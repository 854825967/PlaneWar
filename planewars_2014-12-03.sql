# ************************************************************
# Sequel Pro SQL dump
# Version 4096
#
# http://www.sequelpro.com/
# http://code.google.com/p/sequel-pro/
#
# Host: 119.29.36.204 (MySQL 5.5.30)
# Database: planewars
# Generation Time: 2014-12-03 07:09:30 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;



--
-- Dumping routines (PROCEDURE) for database 'planewars'
--
DELIMITER ;;

# Dump of PROCEDURE proc_changeE
# ------------------------------------------------------------

/*!50003 DROP PROCEDURE IF EXISTS `proc_changeE` */;;
/*!50003 SET SESSION SQL_MODE=""*/;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`127.0.0.1`*/ /*!50003 PROCEDURE `proc_changeE`(IN eid int(32),IN uid varchar(20))
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
END */;;

/*!50003 SET SESSION SQL_MODE=@OLD_SQL_MODE */;;
DELIMITER ;

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
