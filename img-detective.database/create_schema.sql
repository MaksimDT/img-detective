DROP DATABASE IF EXISTS `img_detective`;

CREATE DATABASE `img_detective`
    DEFAULT CHARACTER SET utf8
    DEFAULT COLLATE utf8_general_ci;

USE `img_detective`;

CREATE TABLE `Images` (
    `Id` BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `CreationDate` DATETIME,
    `Description` TEXT NULL
) ENGINE = InnoDB;

CREATE TABLE `ColorHistograms` (
    `ImageId` BIGINT NOT NULL PRIMARY KEY,
    `data` BLOB NOT NULL
) ENGINE = InnoDB;
ALTER TABLE `ColorHistograms`
ADD FOREIGN KEY (`ImageId`)
REFERENCES `Images` (`Id`)
ON UPDATE CASCADE
ON DELETE CASCADE;

CREATE TABLE `FeatureTypes` (
    `Id` INT NOT NULL PRIMARY KEY,
    `Name` TINYTEXT NOT NULL,
    `Description` TEXT
) ENGINE = InnoDB;