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

  public static function queryEntities($field, $conditions, $entity_type = NULL, $object_full = FALSE, $lang = NULL) {
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
    if ($lang) {
      $query->fieldLanguageCondition($field, $lang, '=');
    }
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      $ret[$type] = array_keys($entities);
    }

    if ($result && $entity_type) {
      if ($object_full) {
        return entity_load($entity_type, $ret[$entity_type]);
      }
      else {
        return $ret[$entity_type];
      }
    }

    if ($object_full) {
      foreach ($ret as $type => $ids) {
        $ret[$type] = entity_load($type, $ids);
      }
    }

    return $ret;
  }

  /**
   * Get referenced entities.
   *
   * @param $entity_type
   * @param $entity
   * @param $field_name
   * @param bool $object_full
   * @return array
   */
  public static function entityreferenceEntities($entity_type, $entity, $field_name, $object_full = FALSE) {
    $lang = entity_language($entity_type, $entity);
    $entities = array();
    $items = field_get_items($entity_type, $entity, $field_name, $lang);
    if ($items) {
      $field = field_info_field($field_name);
      $entity_type_ref = $field['settings']['target_type'];
      $entities_id = array();
      foreach ($items as $value) {
        $entities_id[] = $value['target_id'];
      }
      if ($object_full) {
        $entities = entity_load($entity_type_ref, $entities_id);
      }
      else {
        return $entities_id;
      }
    }

    return $entities;
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
  public static function entityreferenceRevertEntities($field, $entity_id, $entity_type = NULL, $object_full = FALSE, $lang = NULL) {
    $query = new EntityFieldQuery();
    if ($entity_type) {
      $query->entityCondition('entity_type', $entity_type);
    }
    $query->fieldCondition($field, 'target_id', $entity_id, '=');
    if ($lang) {
      $query->fieldLanguageCondition($field, $lang, '=');
    }
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      $ret[$type] = array_keys($entities);
    }

    if ($result && $entity_type) {
      if ($object_full) {
        return entity_load($entity_type, $ret[$entity_type]);
      }
      else {
        return $ret[$entity_type];
      }
    }

    if ($object_full) {
      foreach ($ret as $type => $ids) {
        $ret[$type] = entity_load($type, $ids);
      }
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
  public static function queryEntities($entity_type, $property, $value, $object_full = FALSE) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    // Single value or multiple values.
    if (is_array($value)) {
      $query->propertyCondition($property, $value, 'IN');
    }
    else {
      $query->propertyCondition($property, $value, '=');
    }
    $result = $query->execute();
    $ret = array();
    foreach ($result as $type => $entities) {
      $ret[$type] = array_keys($entities);
    }

    if ($result && $entity_type) {
      if ($object_full) {
        return entity_load($entity_type, $ret[$entity_type]);
      }
      else {
        return $ret[$entity_type];
      }
    }

    if ($object_full) {
      foreach ($ret as $type => $ids) {
        $ret[$type] = entity_load($type, $ids);
      }
    }

    return $ret;
  }

}

class KtoolsEntityMetadata {
  
}
