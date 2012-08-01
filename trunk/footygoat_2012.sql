-- Status:9:117:MP_0:footygoat:php:1.24.4::5.1.62-0ubuntu0.10.04.1:1:::utf8:EXTINFO
--
-- TABLE-INFO
-- TABLE|f_fields|14|2400|2012-07-25 20:06:01|MyISAM
-- TABLE|f_leagues|0|1024|2012-07-25 20:06:01|MyISAM
-- TABLE|f_matches|3|2140|2012-07-31 02:00:08|MyISAM
-- TABLE|f_params|1|2068|2012-07-25 20:06:01|MyISAM
-- TABLE|f_teams|0|1024|2012-07-25 20:06:01|MyISAM
-- TABLE|f_timeline|94|4774|2012-07-28 18:16:01|MyISAM
-- TABLE|f_trigger|3|3144|2012-07-25 20:06:01|MyISAM
-- TABLE|f_users|2|2216|2012-07-25 20:06:01|MyISAM
-- TABLE|l_users|0|16384||InnoDB
-- EOF TABLE-INFO
--
-- Dump by MySQLDumper 1.24.4 (http://mysqldumper.net)
/*!40101 SET NAMES 'utf8' */;
SET FOREIGN_KEY_CHECKS=0;
-- Dump created: 2012-07-31 02:00

--
-- Create Table `f_fields`
--

DROP TABLE IF EXISTS `f_fields`;
CREATE TABLE `f_fields` (
  `field_id` tinyint(2) unsigned NOT NULL AUTO_INCREMENT,
  `field_name` varchar(20) NOT NULL,
  `field_tag` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`field_id`)
) ENGINE=MyISAM AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_fields`
--

/*!40000 ALTER TABLE `f_fields` DISABLE KEYS */;
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('1','Goals','goals');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('2','% Time of Possession','possession');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('3','Shots On Goal','gshots');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('4','% Shots On Goal','gshots%');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('5','Shots','shots');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('6','% Shots','shots%');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('7','Corner Kicks','corner');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('8','% Corner Kicks','corner%');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('9','Red Cards','reds');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('10','Yellow Cards','yellows');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('11','% Wins','w');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('12','% Draws','d');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('13','Goals Scored %F','f');
INSERT INTO `f_fields` (`field_id`,`field_name`,`field_tag`) VALUES ('14','Goals Against %A','a');
/*!40000 ALTER TABLE `f_fields` ENABLE KEYS */;


--
-- Create Table `f_leagues`
--

DROP TABLE IF EXISTS `f_leagues`;
CREATE TABLE `f_leagues` (
  `league_id` varchar(40) NOT NULL,
  `league_name` varchar(150) NOT NULL,
  PRIMARY KEY (`league_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Data for Table `f_leagues`
--

/*!40000 ALTER TABLE `f_leagues` DISABLE KEYS */;
/*!40000 ALTER TABLE `f_leagues` ENABLE KEYS */;


--
-- Create Table `f_matches`
--

DROP TABLE IF EXISTS `f_matches`;
CREATE TABLE `f_matches` (
  `match_id` int(11) unsigned NOT NULL,
  `league_id` varchar(40) NOT NULL,
  `hteam` int(11) unsigned NOT NULL,
  `ateam` int(11) unsigned NOT NULL,
  `status` smallint(2) NOT NULL DEFAULT '0',
  `status_ex` varchar(20) DEFAULT NULL,
  `hgoals` tinyint(1) DEFAULT NULL,
  `agoals` tinyint(1) DEFAULT NULL,
  `h1goals` tinyint(1) DEFAULT NULL,
  `a1goals` tinyint(1) DEFAULT NULL,
  `hreds` tinyint(1) DEFAULT NULL,
  `areds` tinyint(1) DEFAULT NULL,
  `hyellows` tinyint(1) DEFAULT NULL,
  `ayellows` tinyint(1) DEFAULT NULL,
  `hshots` smallint(2) DEFAULT NULL,
  `ashots` smallint(2) DEFAULT NULL,
  `hgshots` smallint(2) DEFAULT NULL,
  `agshots` smallint(2) DEFAULT NULL,
  `hcorner` smallint(2) DEFAULT NULL,
  `acorner` smallint(2) DEFAULT NULL,
  `hpossession` tinyint(1) DEFAULT NULL,
  `apossession` tinyint(1) DEFAULT NULL,
  `match_date` datetime DEFAULT NULL,
  `date_view` datetime DEFAULT NULL,
  `mupdate` datetime DEFAULT NULL,
  PRIMARY KEY (`match_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Data for Table `f_matches`
--

/*!40000 ALTER TABLE `f_matches` DISABLE KEYS */;
INSERT INTO `f_matches` (`match_id`,`league_id`,`hteam`,`ateam`,`status`,`status_ex`,`hgoals`,`agoals`,`h1goals`,`a1goals`,`hreds`,`areds`,`hyellows`,`ayellows`,`hshots`,`ashots`,`hgshots`,`agshots`,`hcorner`,`acorner`,`hpossession`,`apossession`,`match_date`,`date_view`,`mupdate`) VALUES ('350120','abc','12','13','0','','2','3','0','0','0','0','3','1','0','0','0','0','0','0','0','0','0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00');
INSERT INTO `f_matches` (`match_id`,`league_id`,`hteam`,`ateam`,`status`,`status_ex`,`hgoals`,`agoals`,`h1goals`,`a1goals`,`hreds`,`areds`,`hyellows`,`ayellows`,`hshots`,`ashots`,`hgshots`,`agshots`,`hcorner`,`acorner`,`hpossession`,`apossession`,`match_date`,`date_view`,`mupdate`) VALUES ('349369','abc','15','16','0','','3','0','0','0','0','0','0','2','0','0','0','0','0','0','0','0','0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00');
INSERT INTO `f_matches` (`match_id`,`league_id`,`hteam`,`ateam`,`status`,`status_ex`,`hgoals`,`agoals`,`h1goals`,`a1goals`,`hreds`,`areds`,`hyellows`,`ayellows`,`hshots`,`ashots`,`hgshots`,`agshots`,`hcorner`,`acorner`,`hpossession`,`apossession`,`match_date`,`date_view`,`mupdate`) VALUES ('350522','abc','123','124','0','','2','2','0','0','0','0','2','5','0','0','0','0','0','0','0','0','0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00');
/*!40000 ALTER TABLE `f_matches` ENABLE KEYS */;


--
-- Create Table `f_params`
--

DROP TABLE IF EXISTS `f_params`;
CREATE TABLE `f_params` (
  `p_name` varchar(20) NOT NULL,
  `p_value` varchar(255) NOT NULL,
  PRIMARY KEY (`p_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Data for Table `f_params`
--

/*!40000 ALTER TABLE `f_params` DISABLE KEYS */;
INSERT INTO `f_params` (`p_name`,`p_value`) VALUES ('dbversion','1.0');
/*!40000 ALTER TABLE `f_params` ENABLE KEYS */;


--
-- Create Table `f_teams`
--

DROP TABLE IF EXISTS `f_teams`;
CREATE TABLE `f_teams` (
  `team_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `team_name` varchar(30) NOT NULL,
  `team_group` smallint(2) DEFAULT 1,
  `team_league` varchar(20) DEFAULT NULL,
  `team_pos` smallint(2) DEFAULT NULL,
  `team_op` smallint(2) DEFAULT NULL,
  `team_ow` smallint(2) DEFAULT NULL,
  `team_od` smallint(2) DEFAULT NULL,
  `team_ol` smallint(2) DEFAULT NULL,
  `team_of` smallint(2) DEFAULT NULL,
  `team_oa` smallint(2) DEFAULT NULL,
  `team_hw` smallint(2) DEFAULT NULL,
  `team_hd` smallint(2) DEFAULT NULL,
  `team_hl` smallint(2) DEFAULT NULL,
  `team_hf` smallint(2) DEFAULT NULL,
  `team_ha` smallint(2) DEFAULT NULL,
  `team_aw` smallint(2) DEFAULT NULL,
  `team_ad` smallint(2) DEFAULT NULL,
  `team_al` smallint(2) DEFAULT NULL,
  `team_af` smallint(2) DEFAULT NULL,
  `team_aa` smallint(2) DEFAULT NULL,
  `team_gd` smallint(2) DEFAULT NULL,
  `team_pts` smallint(2) DEFAULT NULL,
  `team_date` datetime DEFAULT NULL,
  PRIMARY KEY (`team_id`)
) ENGINE=MyISAM AUTO_INCREMENT=100000 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_teams`
--

/*!40000 ALTER TABLE `f_teams` DISABLE KEYS */;
/*!40000 ALTER TABLE `f_teams` ENABLE KEYS */;


--
-- Create Table `f_timeline`
--

DROP TABLE IF EXISTS `f_timeline`;
CREATE TABLE `f_timeline` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `event` int(10) NOT NULL,
  `value` int(10) DEFAULT NULL,
  `team` int(10) DEFAULT NULL,
  `match` int(10) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `lam2lan` (`event`,`value`,`team`,`match`)
) ENGINE=MyISAM AUTO_INCREMENT=95 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_timeline`
--

/*!40000 ALTER TABLE `f_timeline` DISABLE KEYS */;
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('1','1','2','3','4','2012-07-25 21:14:51');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('2','1','2','3','4','2012-07-25 21:15:11');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('3','2','3','4','5','2012-07-26 01:51:34');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('4','4','5','6','7','2012-07-26 01:59:57');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('5','6','5','6','7','2012-07-26 02:03:46');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('6','6','5','6','7','2012-07-26 02:05:26');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('7','0','1','0','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('8','1','0','0','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('9','0','0','1','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('10','1','0','1','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('11','3','2','0','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('12','2','0','0','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('13','3','0','1','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('14','2','0','1','342343','2012-07-26 04:38:25');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('15','0','1','0','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('16','1','1','0','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('17','0','0','1','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('18','1','0','1','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('19','3','0','0','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('20','2','0','0','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('21','3','0','1','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('22','2','0','1','349371','2012-07-28 07:36:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('23','3','1','1','349371','2012-07-28 07:57:00');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('24','0','2','0','349371','2012-07-28 08:01:19');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('25','0','1','0','342343','2012-07-28 08:43:57');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('26','1','0','0','342343','2012-07-28 08:44:17');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('27','0','0','1','342343','2012-07-28 08:44:37');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('28','1','0','1','342343','2012-07-28 08:44:57');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('29','3','2','0','342343','2012-07-28 08:45:17');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('30','2','0','0','342343','2012-07-28 08:45:37');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('31','3','0','1','342343','2012-07-28 08:45:57');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('32','2','0','1','342343','2012-07-28 08:46:18');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('33','0','1','0','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('34','1','0','0','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('35','0','0','1','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('36','1','0','1','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('37','3','2','0','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('38','2','0','0','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('39','3','0','1','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('40','2','0','1','342343','2012-07-28 08:48:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('41','0','1','0','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('42','1','0','0','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('43','0','0','1','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('44','1','0','1','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('45','3','2','0','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('46','2','0','0','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('47','3','0','1','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('48','2','0','1','342343','2012-07-28 08:51:41');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('49','0','1','0','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('50','1','0','0','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('51','0','0','1','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('52','1','0','1','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('53','3','2','0','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('54','2','0','0','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('55','3','0','1','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('56','2','0','1','342343','2012-07-28 08:51:53');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('57','0','1','0','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('58','1','0','0','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('59','0','1','1','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('60','1','0','1','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('61','3','2','0','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('62','2','0','0','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('63','3','0','1','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('64','2','0','1','350120','2012-07-28 08:58:38');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('65','0','3','1','350120','2012-07-28 09:02:27');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('66','3','3','0','350120','2012-07-28 09:02:27');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('67','0','2','0','350120','2012-07-28 09:16:35');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('68','3','1','1','350120','2012-07-28 09:23:45');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('69','0','0','0','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('70','1','0','0','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('71','0','1','1','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('72','1','1','1','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('73','3','1','0','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('74','2','0','0','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('75','3','1','1','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('76','2','0','1','350522','2012-07-28 09:45:02');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('77','0','2','0','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('78','1','2','0','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('79','0','0','1','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('80','1','0','1','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('81','3','0','0','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('82','2','0','0','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('83','3','1','1','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('84','2','0','1','349369','2012-07-28 09:45:52');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('85','3','2','1','349369','2012-07-28 09:53:49');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('86','3','2','1','350522','2012-07-28 10:00:31');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('87','3','3','1','350522','2012-07-28 10:36:42');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('88','0','1','0','350522','2012-07-28 10:38:50');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('89','3','4','1','350522','2012-07-28 10:55:39');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('90','0','3','0','349369','2012-07-28 10:57:01');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('91','0','2','1','350522','2012-07-28 11:00:46');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('92','0','2','0','350522','2012-07-28 18:16:01');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('93','3','2','0','350522','2012-07-28 18:16:01');
INSERT INTO `f_timeline` (`id`,`event`,`value`,`team`,`match`,`date`) VALUES ('94','3','5','1','350522','2012-07-28 18:16:01');
/*!40000 ALTER TABLE `f_timeline` ENABLE KEYS */;


--
-- Create Table `f_trigger`
--

DROP TABLE IF EXISTS `f_trigger`;
CREATE TABLE `f_trigger` (
  `trigger_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `groupid` tinyint(2) unsigned NOT NULL DEFAULT '1',
  `user_id` int(11) unsigned NOT NULL,
  `field_id` tinyint(2) NOT NULL,
  `operater` varchar(2) DEFAULT '',
  `hvalue` varchar(6) DEFAULT NULL,
  `avalue` varchar(6) DEFAULT NULL,
  PRIMARY KEY (`trigger_id`),
  UNIQUE KEY `dkkolap` (`groupid`,`user_id`,`field_id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_trigger`
--

/*!40000 ALTER TABLE `f_trigger` DISABLE KEYS */;
INSERT INTO `f_trigger` (`trigger_id`,`groupid`,`user_id`,`field_id`,`operater`,`hvalue`,`avalue`) VALUES ('1','1','2','11','>=','17','17');
INSERT INTO `f_trigger` (`trigger_id`,`groupid`,`user_id`,`field_id`,`operater`,`hvalue`,`avalue`) VALUES ('2','1','2','1','>','2','2');
INSERT INTO `f_trigger` (`trigger_id`,`groupid`,`user_id`,`field_id`,`operater`,`hvalue`,`avalue`) VALUES ('3','1','2','13','>=','1','0.6');
/*!40000 ALTER TABLE `f_trigger` ENABLE KEYS */;


--
-- Create Table `f_users`
--

DROP TABLE IF EXISTS `f_users`;
CREATE TABLE `f_users` (
  `user_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `user_name` varchar(30) NOT NULL,
  `user_password` varchar(128) NOT NULL,
  `user_email` varchar(50) DEFAULT NULL,
  `user_alert` tinyint(1) DEFAULT '0',
  `user_reg_date` datetime DEFAULT NULL,
  `user_active` tinyint(1) NOT NULL DEFAULT '1',
  `user_newsletter` tinyint(1) NOT NULL DEFAULT '1',
  `user_twitter` varchar(20) NOT NULL DEFAULT '',
  `user_ref` varchar(30) DEFAULT 'register',
  PRIMARY KEY (`user_id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_users`
--

/*!40000 ALTER TABLE `f_users` DISABLE KEYS */;
INSERT INTO `f_users` (`user_id`,`user_name`,`user_password`,`user_email`,`user_alert`,`user_reg_date`,`user_active`,`user_newsletter`,`user_twitter`,`user_ref`) VALUES ('1','admin','21232f297a57a5a743894a0e4a801fc3','admin@footygoat.com','0','2011-11-20 20:43:06','1','1','','register');
INSERT INTO `f_users` (`user_id`,`user_name`,`user_password`,`user_email`,`user_alert`,`user_reg_date`,`user_active`,`user_newsletter`,`user_twitter`,`user_ref`) VALUES ('2','thnam','22c0ac24e6869f37c4b17e27cdce357e','thnam@ifi.edu.vn','0','0000-00-00 00:00:00','1','1','','register');
/*!40000 ALTER TABLE `f_users` ENABLE KEYS */;


--
-- Create Table `l_users`
--

DROP TABLE IF EXISTS `l_users`;
CREATE TABLE `l_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `email` varchar(70) DEFAULT NULL,
  `oauth_uid` int(11) DEFAULT NULL,
  `oauth_provider` varchar(100) DEFAULT NULL,
  `username` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Data for Table `l_users`
--

/*!40000 ALTER TABLE `l_users` DISABLE KEYS */;
/*!40000 ALTER TABLE `l_users` ENABLE KEYS */;

SET FOREIGN_KEY_CHECKS=1;
-- EOB

