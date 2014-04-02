<?php

/**
 * @file
 * Entity related helper.
 */

/**
 * Utility functions related to entity.
 */
class KtoolsEntity {

  /**
   * Get the numeric id from entity_wrapper.
   *
   * @param EntityDrupalWrapper|stdClass|Entity $entity
   *   It can be entity_wrapper or Entity API entity or Core simple StdClass entity.
   * @param $entity_type
   *   (optional) It's no need if passing in entity_wrapper or Entity API entity.
   */
  public static function interId($entity, $entity_type = '') {
    if (is_a($entity, 'EntityDrupalWrapper')) {
      return self::interId($entity->value(), $entity->type());
    }
    else if (is_a($entity, 'Entity')) {
      return $entity->internalIdentifier();
    }
    else if (is_a($entity, 'stdClass') && $entity_type) {
      $entity_class = new Entity((array) $entity, $entity_type);
      return $entity_class->internalIdentifier();
    }
  
    return FALSE;
  }

  /**
   * Generate a list options of defined entity types.
   */
  public static function typeOptions() {
    $entity_type_options = array();
    foreach (entity_get_info() as $entity_type => $entity_info) {
      $entity_type_options[$entity_type] = $entity_info['label'];
    }

    return $entity_type_options;
  }
}

/**
 * Utility functions related to entity field.
 */
class KtoolsEntityField {

  /**
   * Get a list of entities from the reverted entityreference relationship.
   *
   * @param unknown $field
   * @param unknown $entity_id
   * @param unknown $entity_type
   * @param unknown $langcode
   * @return Ambigous <Either, mixed>
   */
  public static function entityreferenceRevertEntities($field, $entity_id, $entity_type = NULL, $langcode = NULL) {
    $query = new EntityFieldQuery();
    if ($entity_type) {
      $query->entityCondition('entity_type', $entity_type);
    }
    $query->fieldCondition($field, 'target_id', $entity_id, '=');
    if ($langcode) {
      $query->fieldLanguageCondition($field, $langcode, '=');
    }
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      //$ret[$entity_type] = array_combine(array_keys($entities), array_keys($entities));
      $ret[$type] = array_keys($entities);
    }
    if ($result && $entity_type) {
      return $ret[$entity_type];
    }

    return $ret;
  }

}

/**
 * Utility functions related to entity property.
 */
class KtoolsEntityProperty {
  
}
