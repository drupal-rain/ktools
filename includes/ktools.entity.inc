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

  /**
   * Generate a list options of defined bundles of an entity_type.
   */
  public static function bundleOptions($entity_type) {
    $entity_info = entity_get_info($entity_type);
    $bundle_options = array();
    foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
      $bundle_options[$bundle_name] = $bundle_info['label'];
    }

    return $bundle_options;
  }

  /**
   * Check if the bundle is within defined bundles of an entity_type.
   */
  public static function bundleExists($entity_type, $bundle) {
    $entity_info = entity_get_info($entity_type);

    return array_key_exists($bundle, $entity_info['bundles']);
  }

  /**
   * Generate an array with bundles as keys and fill NULL as values.
   */
  public static function bundleFillKeys($entity_type, $value = NULL) {
    $entity_info = entity_get_info($entity_type);
    $bundles = array_fill_keys(array_keys($entity_info['bundles']), $value);

    return $bundles;
  }

  /**
   * Generate the common use array of entities keyed by bundle name.
   *
   * @param string $entity_type
   * @param array $entity_ids
   * @param bool $single
   */
  public static function listByBundleKey($entity_type, $entity_ids, $single = TRUE) {
    $entities = entity_load($entity_type, $entity_ids);
    $list = self::bundleFillKeys($entity_type);
    foreach ($entities as $entity_id => $entity) {
      if ($single) {
        $list[$entity->type] = $entity_id;
      }
      else {
        $list[$entity->type][] = $entity_id;
      }
    }

    return $list;
  }

}

/**
 * Utility functions related to entity field.
 */
class KtoolsEntityField {

  public static function queryEntities($field, $conditions, $entity_type = NULL, $langcode = NULL) {
    $query = new EntityFieldQuery();
    if ($entity_type) {
      $query->entityCondition('entity_type', $entity_type);
    }
    $condition_default = array(
      'column' => NULL,
      'value' => NULL,
      'operator' => NULL,
      'delta_group' => NULL,
      'language_group' => NULL,
    );
    foreach ($conditions as $condition) {
      $condition += $condition_default;
      $query->fieldCondition($field, $condition['column'], $condition['value'],
        $condition['operator'], $condition['delta_group'], $condition['language_group']);
    }
    if ($langcode) {
      $query->fieldLanguageCondition($field, $langcode, '=');
    }
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      $ret[$type] = array_keys($entities);
    }
    if ($result && $entity_type) {
      return $ret[$entity_type];
    }

    return $ret;
  }

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

  /**
   * Use EntityFieldQuery to fetch entities.
   */
  public static function queryEntities($entity_type, $property, $value) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type)
      ->propertyCondition($property, $value);
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      $ret[$type] = array_keys($entities);
    }
    if ($result && isset($ret[$entity_type])) {
      return $ret[$entity_type];
    }

    return $ret;
  }

}

class KtoolsEntityMetadata {
  
}
