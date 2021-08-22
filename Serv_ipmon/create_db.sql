CREATE TABLE dresseur (
    dresseur_id INT NOT NULL AUTO_INCREMENT,
    dresseur_name VARCHAR(100) NOT NULL,
    dresseur_pass VARCHAR(100) NOT NULL,
    dresseur_lvl INT,
    dresseur_pos_x INT,
    dresseur_pos_y INT,
    dresseur_map VARCHAR(200),
    PRIMARY KEY(dresseur_id)
);