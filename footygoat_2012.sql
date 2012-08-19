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
  `group` varchar(1) DEFAULT NULL,
  `hteam` int(11) unsigned NOT NULL,
  `ateam` int(11) unsigned NOT NULL,
  `status` smallint(2) NOT NULL DEFAULT '0',
  `minutes` smallint(2) DEFAULT 0,
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
  `hpenalty` tinyint(1) DEFAULT NULL,
  `apenalty` tinyint(1) DEFAULT NULL,
  `match_date` datetime DEFAULT NULL,
  `order` smallint(2) DEFAULT 1,
  PRIMARY KEY (`match_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Data for Table `f_matches`
--




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
INSERT INTO `f_params` (`p_name`,`p_value`) VALUES ('dbversion','2.0');
/*!40000 ALTER TABLE `f_params` ENABLE KEYS */;


--
-- Create Table `f_teams`
--

DROP TABLE IF EXISTS `f_teams`;
CREATE TABLE `f_teams` (
  `team_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `team_name` varchar(30) NOT NULL,
  `team_group` varchar(1) DEFAULT NULL,
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
  `team_updated` tinyint(2) DEFAULT 1,
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
  UNIQUE KEY `lam2lan` (`event`,`team`,`match`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

--
-- Data for Table `f_timeline`
--

/*!40000 ALTER TABLE `f_timeline` DISABLE KEYS */;

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

