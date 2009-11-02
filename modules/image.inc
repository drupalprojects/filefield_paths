<?php
// $Id$

/**
 * @file
 * Provides FileField Paths integration with the Image module.
 */

/**
 * Implements hook_filefield_paths_form_alter().
 */
function image_filefield_paths_form_alter(&$form, &$ffp) {
  if (isset($form['#field']) && $form['#field']['type'] == 'image' && isset($form['instance']['settings']['file_directory'])) {
    $ffp[$form['#field']['field_name']] = array(
      'show' => TRUE,
      'type' => $form['instance']['bundle']['#value'],
      'form_path' => &$form['instance']['ffp_'. $form['#field']['field_name']],
      'file_path_default' => $form['instance']['settings']['file_directory']['#default_value']
    );

    // Create path settings fieldset
    $ffp[$form['#field']['field_name']]['form_path'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image Path settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 1,
    );

    $ffp[$form['#field']['field_name']]['form_path']['file_path'] = $form['instance']['settings']['file_directory'];
    $ffp[$form['#field']['field_name']]['form_path']['file_path']['#title'] = t('File path');
    $form['instance']['settings']['file_directory']['#access'] = FALSE;
  }
}

/**
 * Implements hook_filefield_paths_form_submit().
 */
function image_filefield_paths_form_submit(&$form_state, &$ffp) {
  if (isset($form_state['values']['form_id']) && $form_state['values']['form_id'] != 'node_type_form') {
    $form_state['values']['ffp_' . $form_state['values']['instance']['field_name']] = $form_state['values']['instance']['ffp_' . $form_state['values']['instance']['field_name']];
    $ffp[$form_state['values']['instance']['field_name']] = array(
      'type' => $form_state['values']['instance']['bundle'],
    );

    $form_state['values']['instance']['settings']['file_directory'] = $form_state['values']['ffp_' . $form_state['values']['instance']['field_name']]['file_path'];
  }
}

/**
 * Implements hook_filefield_paths_get_fields().
 */
function image_filefield_paths_get_fields(&$node, &$ffp) {
  if (is_object($node)) {
    // TODO: Replace with field_info_fields() when http://drupal.org/node/613754 is fixed.
    $instances = field_info_instances('node', $node->type);
    $fields = array();
    foreach ($instances as $field_name => $instance) {
      $fields[$field_name] = field_info_field($field_name);
    }

    foreach ($fields as $name => $field) {
      if ($field['type'] == 'image' && isset($node->$field['field_name']) && is_array($node->$field['field_name'])) {

        foreach ($node->{$field['field_name']}['zxx'] as $file) {
          $file = file_load($file['fid']);

          $ffp['#files'][] = array(
            'field' => (array) $file,
            'module' => $field['module'],
            'name' => $field['field_name'],
            //'new' => (!empty($file['filepath']) && !empty($file['data']['process']))
            //  ? TRUE
            //  : FALSE
            'new' => !$file->status,
          );
          //unset($file['data']['process']);

          $ffp['#types'][$field['field_name']] = TRUE;
        }

      }
    }
  }
}

/**
 * Implements hook_filefield_paths_process_file().
 */
//function image_filefield_paths_process_file($new, $file, $settings, $node, $update) {
//  if ($new) {
//    // Delete ImageField thumbnail.
//    if (isset($file['widget']) && $file['widget'] == 'imagefield' && file_exists($thumbnail_source = imagefield_file_admin_thumb_path(array('filepath' => $file['filepath']['old'])))) {
//      file_delete($thumbnail_source);
//    }
//  }
//}

/**
 * Implements hook_filefield_paths_cleanup().
 */
//function image_filefield_paths_cleanup(&$ffp, $paths, $name) {
//  // Check for ImageFields.
//  $imagefield = FALSE;
//  foreach ($ffp['#files'] as $file) {
//    if ($file['name'] == $name) {
//      if (isset($file['widget']) && $file['widget'] == 'imagefield') {
//        $imagefield = TRUE;
//      }
//      break;
//    }
//  }
//  // Cleanup ImageFields temporary thumbnail paths.
//  if ($imagefield) {
//    array_unshift($paths, 'imagefield_thumbs');
//    filefield_paths_cleanup_temp($paths);
//  }
//}
