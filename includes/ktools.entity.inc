<?php
/**
 * Entity utilites class.
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
      $entity_class = new Entity($entity, $entity_type);
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
