<?php

/**
 * @implements hook_rules_data_info().
 */
function ktools_rules_data_info() {
  $data_info['field_item_file'] = array(
    'label' => t('Field File'),
    'wrap' => TRUE,
    'property info' => entity_property_field_item_file_info(),
  );
  $data_info['field_item_image'] = array(
    'label' => t('Field Image'),
    'wrap' => TRUE,
    'property info' => entity_property_field_item_image_info(),
  );

  return $data_info;
}
