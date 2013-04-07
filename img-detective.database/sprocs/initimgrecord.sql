USE img_detective;

DROP PROCEDURE IF EXISTS `initimgrecord`;

delimiter //

CREATE PROCEDURE `initimgrecord`()
BEGIN
    INSERT INTO `Images`
    (`Path`, `CreationDate`, `Description`)
    VALUES
    (NULL, NOW(), NULL);

    SELECT LAST_INSERT_ID() as imgid;
END//

delimiter ;