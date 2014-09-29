<?php

class KtoolsModule {

  public static function getWeight($module) {
    $query = db_select('system');
    $query->addField('system', 'weight');
    $query->condition('name', $module);
    $result = $query->execute();

    return $result->fetchField();
  }

  public static function setWeight($module, $weight) {
    db_query("UPDATE {system} SET weight = :weight WHERE name = :module", array(
      ':weight' => $weight,
      ':module' => $module,
    ));
  }

  public static function setWeightAfter($module, $module_target) {
    $weight_target = self::getWeight($module_target);
    self::setWeight($module, $weight_target + 1);
  }

}
