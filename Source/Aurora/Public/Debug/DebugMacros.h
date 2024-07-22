#pragma once

#include "DrawDebugHelpers.h"

#pragma region DRAW GEOMETRY

// Sphere
#define DEBUG_DRAW_SPHERE(Location)								if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 5.f, 5.f, FColor::Red, false, 3.f)
#define DEBUG_DRAW_SPHERE_SingleFrame(Location)					if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DEBUG_DRAW_SPHERE_RADIUS_SingleFrame(Location, Radius)	if (GetWorld()) DrawDebugSphere(GetWorld(), Location, Radius, 12, FColor::Red, false, -1.f);

#define DEBUG_DRAW_LINE(StartLocation, EndLocation)								if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f)
#define DEBUG_DRAW_LINE_SingleFrame(StartLocation, EndLocation)					if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DEBUG_DRAW_LINE_COLOR_SingleFrame(StartLocation, EndLocation, Color)	if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, -1.f, 0, 1.f);
#define DEBUG_DRAW_POINT(Location)												if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true)
#define DEBUG_DRAW_POINT_THICKNESS(Location, Thickness)							if (GetWorld()) DrawDebugPoint(GetWorld(), Location, Thickness, FColor::Red, true)
#define DEBUG_DRAW_POINT_SingleFrame(Location)									if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f);
#define DEBUG_DRAW_POINT_COLOR_SingleFrame(Location, Color)						if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, Color, false, -1.f);

#define DEBUG_DRAW_VECTOR(StartLocation, EndLocation)			if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	}

#define DEBUG_DRAW_VECTOR_SingleFrame(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false, -1.f); \
	}

#pragma endregion

#pragma region LOG

// INT
#define DEBUG_MESSAGE_INT(Value)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%i"),Value));
#define DEBUG_MESSAGE_INT_COLOR(Value, Color)	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT("%i"),Value));

// FLOAT
#define DEBUG_MESSAGE_FLOAT(Value)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%f"),Value));
#define DEBUG_MESSAGE_FLOAT_SingleFrame(Value)	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%f"),Value));
#define DEBUG_MESSAGE_FLOAT_COLOR(Value, Color) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT("%f"),Value));

// BOOL
#define DEBUG_MESSAGE_BOOL(Value)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%hd"),Value));
#define DEBUG_MESSAGE_BOOL_COLOR(Value, Color)	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT("%hd"),Value));

// Vector
#define DEBUG_MESSAGE_VECTOR(Vector)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s"), Vector));
#define DEBUG_MESSAGE_VECTOR_COLOR(Vector, Color)	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT("%s"), Vector);

// Strings
#define DEBUG_MESSAGE_STRING(StringValue)									if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s"), *StringValue));
#define DEBUG_MESSAGE_STRING_SingleFrame(StringValue)						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%s"), *StringValue));
#define DEBUG_MESSAGE_STRING_COLOR(StringValue, Color)						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT("%s"), *StringValue));
#define DEBUG_MESSAGE_STRING_COLOR_KEY(StringValue, Color, Key)				if (GEngine) GEngine->AddOnScreenDebugMessage(Key, 5.f, Color, FString::Printf(TEXT("%s"), *StringValue));
#define DEBUG_MESSAGE_STRING_COLOR_SingleFrame(StringValue, Color)			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, Color, FString::Printf(TEXT("%s"), *StringValue));
#define DEBUG_MESSAGE_STRING_COLOR_SingleFrame_KEY(StringValue, Color, Key)	if (GEngine) GEngine->AddOnScreenDebugMessage(Key, 0.f, Color, FString::Printf(TEXT("%s"), *StringValue));

// Text
#define DEBUG_MESSAGE_TEXT(TextValue)							if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT(TextValue)));
#define DEBUG_MESSAGE_TEXT_COLOR(TextValue, Color)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT(TextValue)));
#define DEBUG_MESSAGE_TEXT_SingleFrame(TextValue)				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT(TextValue)));
#define DEBUG_MESSAGE_TEXT_COLOR_SingleFrame(TextValue, Color)	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, Color, FString::Printf(TEXT(TextValue)));

#pragma endregion