const QUESTIONS = [
  {
    "id": "F1",
    "priority": 1,
    "category": "Scope",
    "question": "Zu meinem Verständnis: Das Projekt umfasst die Planung und Steuerung eines begrenzten Archivpiloten — nicht die vollständige Migration des FRS-Gesamtarchivs. Ist diese Abgrenzung so korrekt? Welche Nicht-Ziele soll ich ausdrücklich in der Zielvereinbarung aufnehmen?",
    "modelAnswer": "Abgrenzung: Pilot, keine Gesamtmigration. Nicht-Ziele: keine vollständige Bestandsmigration, kein Austausch aller produktiven Sendesysteme, keine unautorisierten Änderungen an Produktivsystemen, keine rechtswidrige Veröffentlichung.",
    "options": [
      "Pilotprojekt mit begrenztem Scope, keine Gesamtmigration",
      "Vollständige Migration des FRS-Gesamtarchivs",
      "Nur Datenanalyse ohne Pilot",
      "Proof of Concept ohne Dokumentation"
    ],
    "correctIndex": 0,
    "keywords": [
      "Pilot",
      "Nicht-Ziele",
      "Abgrenzung",
      "Scope"
    ],
    "difficulty": 1
  },
  {
    "id": "F2",
    "priority": 1,
    "category": "Type",
    "question": "Liegt der Schwerpunkt auf der Projektleitung eines IT-Projekts mit technischem Pilot, oder auf einer Machbarkeitsstudie mit Konzept und Empfehlung? Meine Eigenleistung umfasst beides — Projektleitung und dokumentierte Pilotteile. Wie soll das Verhältnis in der Zielvereinbarung beschrieben werden?",
    "modelAnswer": "Beides: Projektleitung (§12: Projekte einrichten, durchführen, auflösen; Projekte leiten) + dokumentierter technischer Pilot. Verhältnis: Leitung im Mittelpunkt, Implementierung als Teilprojekt.",
    "options": [
      "Projektleitung + dokumentierter technischer Pilot",
      "Nur Machbarkeitsstudie",
      "Nur technische Implementierung",
      "Reine Beratung ohne Pilot"
    ],
    "correctIndex": 0,
    "keywords": [
      "Projektleitung",
      "Machbarkeitsstudie",
      "IT-Projekt",
      "Eigenleistung"
    ],
    "difficulty": 2
  },
  {
    "id": "F3",
    "priority": 1,
    "category": "Documentation",
    "question": "Welche Art von Dokumentation wird erwartet — Bericht, Mappe, digitale Ablage? Gibt es Vorgaben zu Umfang, Gliederung oder Seitenumfang? Soll die Dokumentation den Projektverlauf darstellen und analytisch bewerten (gemäß § 12)?",
    "modelAnswer": "Erwartet: Projektdokumentation (Verlauf + analytische Bewertung). Handreichung: 8-10h oder max 20% der Gesamtzeit. Bei 70h = 12h (17%) — im Plan enthalten. Ausschuss muss bestätigen.",
    "options": [
      "Projektdokumentation mit Verlauf und analytischer Bewertung",
      "Kurzer Statusbericht (1-2 Seiten)",
      "Nur technische Dokumentation",
      "Präsentation ohne Schriftstück"
    ],
    "correctIndex": 0,
    "keywords": [
      "Dokumentation",
      "Umfang",
      "Gliederung",
      "§12",
      "Handreichung"
    ],
    "difficulty": 2
  },
  {
    "id": "F4",
    "priority": 1,
    "category": "Timeline",
    "question": "Wann ist der verbindliche Abgabetermin? Gibt es eine Mindestdauer für die Projektphase? Zwischen Beratung und Abgabe dürfen nach Verordnung maximal 12 Monate liegen — mein Plan sieht den Abschluss innerhalb dieses Rahmens vor.",
    "modelAnswer": "Abgabetermin Teil der Zielvereinbarung (§4 VO). Ohne festen Termin keine belastbare Planung. 70h-Verteilung hängt vom Zeitrahmen ab. Max 12 Monate zwischen Beratung und Abgabe.",
    "options": [
      "Abgabetermin als Teil der Zielvereinbarung, max. 12 Monate",
      "Kein fester Termin nötig",
      "Abgabe nach Belieben",
      "Termin wird von IHK vorgegeben"
    ],
    "correctIndex": 0,
    "keywords": [
      "Abgabetermin",
      "Zielvereinbarung",
      "12 Monate",
      "Zeitrahmen"
    ],
    "difficulty": 2
  },
  {
    "id": "F5",
    "priority": 1,
    "category": "Focus",
    "question": "Welche Schwerpunkte soll die Projektarbeit aus Ihrer Sicht setzen? Soll ich besonderes Gewicht auf Ist-Analyse, Alternativenbewertung, Wirtschaftlichkeit oder die technische Pilotumsetzung legen?",
    "modelAnswer": "Ausschuss kann Schwerpunkte setzen (§12/§4). Wer das vorher weiß, kann 70h entsprechend priorisieren.",
    "options": [
      "Ist-Analyse, Alternativenbewertung, Wirtschaftlichkeit, Pilot",
      "Nur technische Implementierung",
      "Nur Dokumentation",
      "Nur Beratung"
    ],
    "correctIndex": 0,
    "keywords": [
      "Schwerpunkte",
      "Priorisierung",
      "70h",
      "Ausschuss"
    ],
    "difficulty": 2
  },
  {
    "id": "F6",
    "priority": 2,
    "category": "Formal",
    "question": "Das mir am 01.10. übermittelte Merkblatt enthält eine Terminübersicht mit Daten aus 2025. Gilt diese Fassung für meinen Durchgang weiterhin, oder liegt eine aktualisierte Fassung für 2026 vor? Insbesondere die formalen Dokumentationsvorgaben möchte ich auf dem richtigen Stand haben.",
    "modelAnswer": "Merkblatt wurde mit Nachreichungspaket gesendet. Veraltete Fassung könnte falsche Formalvorgaben enthalten. Nicht ungeprüft als verbindlich darstellen.",
    "options": [
      "Aktualisierte Fassung für 2026 prüfen",
      "2025-Fassung gilt weiterhin",
      "Merkblatt ist nicht relevant",
      "Keine Formalvorgaben vorhanden"
    ],
    "correctIndex": 0,
    "keywords": [
      "Merkblatt",
      "2025",
      "2026",
      "Formalvorgaben"
    ],
    "difficulty": 1
  },
  {
    "id": "F7",
    "priority": 2,
    "category": "Formal",
    "question": "Ihre Einladung nennt die Prüfungsnummer 130465. In meinen Unterlagen taucht daneben die Nummer 615951 auf. Sind beide Bezüge korrekt — die eine als Prüfungs-, die andere als Teilnehmerkennung — oder soll ich eine davon korrigieren?",
    "modelAnswer": "130465 = Prüfungsnummer (Einladung). 615951 = Prüflings-ID / Teilnehmerbezug. Beide im selben Schreiben. Keine stillschweigende Gleichsetzung.",
    "options": [
      "130465 = Prüfungsnummer, 615951 = Teilnehmerkennung",
      "Beide sind Prüfungsnummern",
      "Nur 130465 ist korrekt",
      "Nur 615951 ist korrekt"
    ],
    "correctIndex": 0,
    "keywords": [
      "Prüfungsnummer",
      "Teilnehmerkennung",
      "130465",
      "615951"
    ],
    "difficulty": 1
  },
  {
    "id": "F8",
    "priority": 2,
    "category": "DataAccess",
    "question": "Für die Ist-Analyse benötige ich Zugang zu den FRS-Datenstrukturen (Drupal, Dateisystem, Metadaten). Wer ist die entscheidungsberechtigte Person auf FRS-Seite für diese Freigaben? Und ab wann darf ich mit der Ist-Analyse beginnen — vor oder erst nach der Zielvereinbarung?",
    "modelAnswer": "Ohne Datenzugang keine Ist-Analyse. 10h Initiierungsphase setzen realen Zugang voraus. Wer freigibt, muss vorher geklärt sein.",
    "options": [
      "Entscheidungsberechtigte Person + Zugang vor Zielvereinbarung",
      "Zugang erst nach Projektabschluss",
      "Kein Zugang nötig",
      "Zugang über allgemeine Anfrage"
    ],
    "correctIndex": 0,
    "keywords": [
      "Datenzugang",
      "Ist-Analyse",
      "FRS",
      "Freigabe",
      "Entscheidungsberechtigt"
    ],
    "difficulty": 2
  },
  {
    "id": "F9",
    "priority": 3,
    "category": "Coach",
    "question": "Ich habe Roman Kalex als betriebliche IT-Ansprechperson und Carsten Vordermeier als externen Coach benannt. Muss die betriebliche Rolle von FRS-Seite schriftlich bestätigt werden, oder genügt die Benennung im Antrag?",
    "modelAnswer": "Keine generelle Pflicht in allgemeinen Bestimmungen ersichtlich. Aber: IHK kann eigene Formblätter/Nachreichungen fordern. Coach-Rolle ersetzt nicht FRS-Datenfreigabe oder Beauftragung.",
    "options": [
      "Benennung genügt, aber IHK kann Nachreichung fordern",
      "Schriftliche Bestätigung immer erforderlich",
      "Keine Bestätigung nötig",
      "Coach ersetzt FRS-Freigabe"
    ],
    "correctIndex": 0,
    "keywords": [
      "Coach",
      "Roman Kalex",
      "Carsten Vordermeier",
      "Schriftlich",
      "Formblätter"
    ],
    "difficulty": 1
  },
  {
    "id": "F10",
    "priority": 3,
    "category": "ChangeControl",
    "question": "Was gilt, wenn sich Projektumfang oder Anforderungen im Projektverlauf ändern? Muss jede Änderung mit dem Ausschuss abgestimmt werden, oder gibt es eine Untergrenze für anmeldepflichtige Abweichungen?",
    "modelAnswer": "Scope Creep ist typisches Risiko. Geklärtes Änderungsverfahren schützt vor Überraschungen bei der Abnahme.",
    "options": [
      "Geklärtes Änderungsverfahren schützt vor Überraschungen",
      "Jede Änderung muss genehmigt werden",
      "Keine Änderungen erlaubt",
      "Änderungen sind frei möglich"
    ],
    "correctIndex": 0,
    "keywords": [
      "Änderungsverfahren",
      "Scope Creep",
      "Abweichungen",
      "Abnahme"
    ],
    "difficulty": 2
  },
  {
    "id": "F11",
    "priority": 3,
    "category": "Evaluation",
    "question": "Nach welchen Kriterien wird die Projektarbeit bewertet? Gibt es eine Bewertungsrubrik, die ich vorab einsehen könnte, um die Dokumentation entsprechend auszurichten?",
    "modelAnswer": "Handreichungen nennen Argumentationslogik, Problemlösung, eigene gedankliche Leistung, Aufbau. Konkrete Rubrik wäre hilfreich.",
    "options": [
      "Bewertungskriterien vorab einsehen",
      "Kriterien werden erst bei Abgabe bekannt",
      "Nur technische Umsetzung zählt",
      "Nur Dokumentation zählt"
    ],
    "correctIndex": 0,
    "keywords": [
      "Bewertung",
      "Kriterien",
      "Rubrik",
      "Argumentationslogik"
    ],
    "difficulty": 2
  }
];