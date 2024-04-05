-- Adminer 4.8.1 MySQL 8.3.0 dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

SET NAMES utf8mb4;

CREATE DATABASE `wmm` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `wmm`;

DROP TABLE IF EXISTS `transactions`;
CREATE TABLE `transactions` (
  `date` datetime NOT NULL,
  `description` varchar(200) NOT NULL,
  `category` varchar(200) DEFAULT NULL,
  `amount` double NOT NULL,
  PRIMARY KEY (`date`),
  KEY `description` (`description`),
  KEY `category` (`category`),
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


-- 2024-04-05 10:43:47
