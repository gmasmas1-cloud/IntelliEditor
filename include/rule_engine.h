// include/rule_engine.h
#ifndef RULE_ENGINE_H
#define RULE_ENGINE_H

#include "rule_parser.h"  // Pour RuleSet
#include "rule_checker.h" // Pour RuleReport

// Fonction principale : vérifie toutes les règles et génère un rapport
RuleReport* check_all_rules(RuleSet *ruleset, const char *text);

// Libère la mémoire du rapport
void free_rule_report(RuleReport *report);

#endif