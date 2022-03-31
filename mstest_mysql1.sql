DROP TABLE IF EXISTS `mstest`;

CREATE TABLE `mstest` (
  `id` bigint(20) UNSIGNED NOT NULL,
  `firstname` varchar(50) NOT NULL,
  `lastname` varchar(50) NOT NULL,
  `birthday` date NOT NULL,
  `dtext` text NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `created_by` varchar(50)  NOT NULL,
  `updated_at` timestamp NULL DEFAULT current_timestamp(),
  `updated_by` varchar(50) DEFAULT NULL
) ENGINE=InnoDB;

--
-- Indizes der exportierten Tabellen
--

--
-- Indizes für die Tabelle `mstest`
--
ALTER TABLE `mstest`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT für exportierte Tabellen
--

--
-- AUTO_INCREMENT für Tabelle `mstest`
--
ALTER TABLE `mstest`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;
COMMIT;


INSERT INTO `mstest` (`firstname`, `lastname`, `birthday`, `dtext`, `created_by`) 
	VALUES('Reinhard', 'Pagitsch', '1965-09-08', 'nur ein Test', 'reinhardp'),
	('Peter', 'Pagitsch', '1971-09-06', 'nur ein Test 1', 'reinhardp'),
	('Gerti', 'Pagitsch', '1943-01-03', 'nur ein Test 2', 'reinhardp'),
	('Martin', 'Pagitsch', '1991-10-29', 'nur ein Test 3', 'reinhardp'),
	('Marina', 'Pagitsch', '1969-10-08', 'nur ein Test 4', 'reinhardp'),
	('Petra', 'Pagitsch', '1980-08-06', 'nur ein Test 5', 'reinhardp'),
	('Hide', 'Pagitsch', '1942-01-03', 'nur ein Test 6', 'reinhardp'),
	('Trude', 'Pagitsch', '1940-11-10', 'nur ein Test 7', 'reinhardp'),
	('Paul', 'Pagitsch', '1960-11-08', 'nur ein Test 8', 'reinhardp'),
	('Manfred',	'Pagitsch', '1955-10-08', 'nur ein Test 9', 'reinhardp'),
	('Gerti', 'Müller', '1957-10-08', 'nur ein Test 9', 'reinhardp')
