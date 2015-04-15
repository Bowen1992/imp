/**
 * \file IMP/em2d/PCAFitRestraint.cpp
 * \brief fast scoring of fit between Particles in 3D and 2D class averages
 *
 * \authors Dina Schneidman
 * Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/em2d/PCAFitRestraint.h>

#include <IMP/em2d/internal/Projection.h>
#include <IMP/em2d/internal/ImageTransform.h>

IMPEM2D_BEGIN_NAMESPACE

PCAFitRestraint::PCAFitRestraint(Particles particles,
                                 const std::vector<std::string>& image_files,
                                 double pixel_size, double resolution,
                                 unsigned int projection_number, bool reuse_direction)
  : Restraint(particles[0]->get_model(), "PCAFitRestraint%1%"),
    ps_(particles),
    pixel_size_(pixel_size),
    resolution_(resolution),
    projection_number_(projection_number),
    projector_(ps_, projection_number, pixel_size, resolution),
    reuse_direction_(reuse_direction),
    counter_(0.0)
{

  // read and process the images
  for (unsigned int i = 0; i < image_files.size(); i++) {
    internal::Image2D<> image(image_files[i]);
    image.get_largest_connected_component();
    image.pad((int)(image.get_width() * 1.4), (int)(image.get_height() * 1.4));
    image.center();
    image.average();
    image.stddev();
    image.compute_PCA();
    images_.push_back(image);
  }

  internal::Image2D<>::write_PGM(images_, "images.pgm");
}

double PCAFitRestraint::unprotected_evaluate(
                                  IMP::DerivativeAccumulator *accum) const {
  IMP_UNUSED(accum);

  counter_++; // increase counter

  // generate projections
  boost::ptr_vector<internal::Projection> projections;
  if(reuse_direction_ && counter_ % 100 != 0 &&
     best_projections_axis_.size() == images_.size()) {
    projector_.compute_projections(best_projections_axis_,
                                   IMP_DEG_2_RAD(20),  // 20 degrees for now
                                   projections,
                                   images_[0].get_height());
  } else {
    projector_.compute_projections(projections, images_[0].get_height());
  }
  IMP_LOG_VERBOSE(projections.size() << " projections were created"
                  << std::endl);

  // process projections
  for (unsigned int i = 0; i < projections.size(); i++) {
    projections[i].get_largest_connected_component();
    projections[i].center();
    projections[i].average();
    projections[i].stddev();
    projections[i].compute_PCA();
  }

  // score each image against projections
  double total_score = 0;
  double area_threshold = 0.4;
  PCAFitRestraint* non_const_this = const_cast<PCAFitRestraint *>(this);
  non_const_this->best_projections_.clear();
  non_const_this->best_projections_axis_.clear();
  for (unsigned int i = 0; i < images_.size(); i++) {
    internal::ImageTransform best_transform;
    best_transform.set_score(0.0);
    int best_projection_id = 0;
    for (unsigned int j = 0; j < projections.size(); j++) {
      // do not align images with more than X% area difference
      double area_score = std::abs(images_[i].segmented_pixels() -
                                   projections[j].segmented_pixels()) /
        (double)std::max(images_[i].segmented_pixels(),
                         projections[j].segmented_pixels());
      if (area_score > area_threshold) continue;

      internal::ImageTransform curr_transform =
        images_[i].pca_align(projections[j]);
      curr_transform.set_area_score(area_score);
      if (curr_transform.get_score() > best_transform.get_score()) {
        best_transform = curr_transform;
        best_projection_id = projections[j].get_id();
      }
    }
    IMP_LOG_VERBOSE("Image " << i << " Best projection " << best_projection_id
                    << " " << best_transform << std::endl);
    total_score += -log(best_transform.get_score());

    // save best projections and their projection axis
    internal::Image2D<> transformed_image;
    projections[best_projection_id]
        .rotate_circular(transformed_image, best_transform.get_angle());
    transformed_image.translate(best_transform.get_x(), best_transform.get_y());
    non_const_this->best_projections_.push_back(transformed_image);
    non_const_this->best_projections_axis_.push_back(projections[best_projection_id].get_axis());
  }
  return total_score;
}

void PCAFitRestraint::write_best_projections(std::string file_name,
                                             bool evaluate) {
  if(best_projections_.size() == 0 || evaluate) {
    unprotected_evaluate(nullptr);
  }
  internal::Image2D<>::write_PGM(best_projections_, file_name);
}

IMPEM2D_END_NAMESPACE
